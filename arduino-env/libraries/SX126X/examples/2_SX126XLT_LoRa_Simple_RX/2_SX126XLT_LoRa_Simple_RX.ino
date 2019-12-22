/*
******************************************************************************************************

  LoRaTracker Programs for Arduino

  Copyright of the author Stuart Robinson 24/10/19

  http://www.LoRaTracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without
  the explicit permission of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the
  intended purpose and free from errors.

  The program listens for incoming packets using the LoRa settings in the 'Settings.h' file. The pins to
  access the SX126X need to be defined in the 'Settings.h' file also.

  The program is a simple receiver. There is a printout of the valid packets received, these are assumed
  to be in ASCII printable text. The LED will flash for each packet received and the buzzer will sound,
  if fitted.

  Changes:

  To Do:


******************************************************************************************************/


#define programversion "V1.0"
#define Serial_Monitor_Baud 115300

#include <SPI.h>
#include <SX126XLT.h>
#include "Settings.h"


SX126XClass SX126XLT;

uint32_t RXpacketCount;
uint32_t errors;
bool ENABLEBUZZER;

uint8_t RXBUFFER[RXBUFFER_SIZE];

uint8_t RXPacketL;                               //stores length of packet received
int8_t  PacketRSSI;                              //stores RSSI of received packet
int8_t  PacketSNR;                               //stores signal to noise ratio of received packet


void loop()
{
  SX126XLT.setRx(10000);                         //set 10 second RX timeout

  while (!digitalRead(DIO1));                    //wait for RxDone or timeout interrupt activating DIO1

  delay(250);                                    //allow time for all IRQs to be set

  digitalWrite(LED1, HIGH);

  if (ENABLEBUZZER)
  {
    digitalWrite(BUZZER, HIGH);
  }

  SX126XLT.readPacketReceptionLoRa();
  RXPacketL = SX126XLT.readRXPacketL();
  PacketRSSI = SX126XLT.readPacketRSSI();
  PacketSNR = SX126XLT.readPacketSNR();

  if (SX126XLT.readIrqStatus() == (IRQ_RX_DONE + IRQ_PREAMBLE_DETECTED + IRQ_HEADER_VALID))
  {
    packet_is_OK();
  }
  else
  {
    packet_is_Error();
  }

  digitalWrite(LED1, LOW);

  if (ENABLEBUZZER)
  {
    digitalWrite(BUZZER, LOW);
  }
  Serial.println();
}


void packet_is_OK()
{
  uint16_t IRQStatus;
  uint8_t len;

  RXpacketCount++;
  len = SX126XLT.readPacketLoRa(RXBUFFER, RXBUFFER_SIZE);         //read the actual packet, maximum size specified in RXBUFFER_SIZE

  if (len == 0)
  {
    Serial.print(F("Buffer Error"));
  }
  else
  {
    SX126XLT.printASCIIPacket(RXBUFFER, len);                     //len same as RXPacketL
  }

  Serial.print(",RSSI,");

  Serial.print(PacketRSSI);
  Serial.print(F("dBm,SNR,"));
  Serial.print(PacketSNR);
  Serial.print(F("dB,Length,"));
  Serial.print(len);
  Serial.print(F(",Packets,"));
  Serial.print(RXpacketCount);
  Serial.print(F(",Errors,"));
  Serial.print(errors);

  IRQStatus = SX126XLT.readIrqStatus();
  Serial.print(F(",IRQreg,"));
  Serial.print(IRQStatus, HEX);
}


void packet_is_Error()
{
  uint16_t IRQStatus;
  uint8_t len;
  IRQStatus = SX126XLT.readIrqStatus();                    //get the IRQ status
  errors++;

  if (IRQStatus & IRQ_RX_TX_TIMEOUT)
  {
    Serial.print(F("RX Timeout"));
  }
  else
  {
    Serial.print(F("PacketError,"));

    Serial.print(F(",RSSI"));
    Serial.print(PacketRSSI);
    Serial.print(F("dBm,SNR,"));
    Serial.print(PacketSNR);

    Serial.print(F("dB,Length,"));
    Serial.print(RXPacketL);
    Serial.print(F(",IRQreg,"));
    Serial.print(IRQStatus, HEX);
    SX126XLT.printIrqStatus();
    digitalWrite(LED1, LOW);

    if (ENABLEBUZZER)
    {
      digitalWrite(BUZZER, LOW);
      delay(100);
      digitalWrite(BUZZER, HIGH);
    }
  }
}


void PrintPacket()
{
  Serial.write(RXBUFFER, RXPacketL);                  //print whole packet, ASCII assumed
}


void led_Flash(uint16_t flashes, uint16_t delaymS)
{
  unsigned int index;

  for (index = 1; index <= flashes; index++)
  {
    digitalWrite(LED1, HIGH);
    delay(delaymS);
    digitalWrite(LED1, LOW);
    delay(delaymS);
  }
}


void setup_LoRa()
{
  SX126XLT.setStandby(MODE_STDBY_RC);
  SX126XLT.setRegulatorMode(USE_DCDC);
  SX126XLT.setPaConfig(0x04, HPMAXAUTO, DEVICE_SX1262);
  SX126XLT.setDIO3AsTCXOCtrl(TCXO_CTRL_3_3V);
  SX126XLT.calibrateDevice(ALLDevices);                      //is required after setting TCXO
  SX126XLT.calibrateImage(Frequency);
  SX126XLT.setDIO2AsRfSwitchCtrl();
  SX126XLT.setPacketType(PACKET_TYPE_LORA);
  SX126XLT.setRfFrequency(Frequency, Offset);
  SX126XLT.setModulationParams(SpreadingFactor, Bandwidth, CodeRate, LDRO_AUTO);
  SX126XLT.setBufferBaseAddress(0, 0);
  SX126XLT.setPacketParams(12, LORA_PACKET_VARIABLE_LENGTH, 255, LORA_CRC_ON, LORA_IQ_NORMAL);
  SX126XLT.setDioIrqParams(IRQ_RADIO_ALL, (IRQ_RX_DONE + IRQ_RX_TX_TIMEOUT), 0, 0);   //set for IRQ on RX done and timeout on DIO1
  //the appropriate syncword can be set here, the default is LORA_MAC_PRIVATE_SYNCWORD (0x12)
  SX126XLT.setSyncWord(LORA_MAC_PRIVATE_SYNCWORD); //0x1424 written but 0x12 used
  //SX126XLT.setSyncWord(LORA_MAC_PUBLIC_SYNCWORD);  //0x3444 written but 0x34 used as needed for LoRaWAN 
}


void setup(void)
{
  pinMode(LED1, OUTPUT);
  led_Flash(2, 125);

  Serial.begin(Serial_Monitor_Baud);
  Serial.println();

  Serial.println();
  Serial.print(F(__TIME__));
  Serial.print(F(" "));
  Serial.println(F(__DATE__));
  Serial.println(F(programversion));
  Serial.println();
  Serial.println();
  Serial.println(F("2_SX126XLT_LoRa_Simple_RX Starting"));
  Serial.println();

  if (BUZZER >= 0)
  {
    ENABLEBUZZER = true;
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);
    Serial.println(F("BUZZER Enabled"));
  }
  else
  {
    ENABLEBUZZER = false;
    Serial.println(F("BUZZER Not Enabled"));
  }

  SPI.begin();
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

  if (SX126XLT.begin(NSS, NRESET, RFBUSY, DIO1, DIO2, DIO3))
  {
    Serial.println(F("Device found"));
    led_Flash(2, 125);
    delay(1000);
  }
  else
  {
    Serial.println(F("No device responding"));
    while (1)
    {
      led_Flash(50, 50);                                            
    }
  }

    setup_LoRa();


  Serial.print(F("Receiver ready - RXBUFFER_SIZE "));
  Serial.println(RXBUFFER_SIZE);
  SX126XLT.printLoraSettings();
  Serial.println();
  Serial.println();
}



