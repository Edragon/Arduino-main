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

  Changes:

  To Do:

******************************************************************************************************
*/

/*
******************************************************************************************************

  Program Operation

  The LoRa settings to use are specified in the 'Settings.h' file.

  This program tests the sleep mode and register retention functions of the SX1262 in sleep mode. 
  A packet is sent and the SX1262 is put to sleep with register values retained. It should then be
  able to be woken up from sleep with registers intact and be able to transmit another packet without
  the need to reload all the LoRa settings. 

  Tested on an ATmega328P board that has a sleep current of 6.4uA. The sleep current is much the same
  with the SX1262 in sleep mode. 




******************************************************************************************************
*/

#define programversion "V1.0"
#define Serial_Monitor_Baud 115200

#include <avr/wdt.h>                 //watchdog timer
#include <SPI.h>
#include <LowPower.h>                //https://github.com/rocketscream/Low-Power
#include "PinChangeInterrupt.h"      //https://github.com/NicoHood/PinChangeInterrupt

#include <SX126XLT.h>
#include "Settings.h"

SX126XClass SX126XLT;

boolean SendOK;
int8_t TestPower;
uint8_t TXPacketL;

uint8_t buff[13] = "Hello Worldx";

void loop()
{

  digitalWrite(LED1, HIGH);
  Serial.print(TXpower);
  Serial.print(F("dBm "));
  Serial.print(F("TestPacket1> "));
  Serial.flush();

  buff[11] = '1';                  //to identify packet 1

  if (Send_Test_Packet())
  {
    packet_is_OK();
  }
  else
  {
    packet_is_Error();
  }
  Serial.println();
  delay(packet_delay);
  
  uint8_t buffer[1];
  
  SX126XLT.setSleep(CONFIGURATION_RETENTION);                        //preserve register settings in sleep.  
  Serial.println(F("Sleeping zzzzz...."));
  Serial.flush();
  digitalWrite(LED1, LOW);
  
  sleep_timed(10);                                       
  
  Serial.println(F("Awake !"));
  Serial.flush();
  digitalWrite(LED1, HIGH);
  SX126XLT.wake();

  Serial.print(F("TestPacket2> "));
  Serial.flush();

  buff[11] = '2';                       //to identify packet 2  

  if (Send_Test_Packet())
  {
    packet_is_OK();
  }
  else
  {
    packet_is_Error();
  }
  Serial.println();
  delay(packet_delay);
 
}


void sleep_timed(uint32_t seconds)
{
  uint32_t i; 
  attachPCINT(digitalPinToPCINT(SWITCH1), wakeUp, LOW);   //This is a hardware interrupt
  for (i = 1; i <= seconds; i++)
  {
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);         //sleep 1 seconds
  }
  detachPCINT(digitalPinToPCINT(SWITCH1));
}


void wakeUp()
{
  //handler for the interrupt.
}


void packet_is_OK()
{
  Serial.print(F(" "));
  Serial.print(TXPacketL);
  Serial.print(F(" Bytes SentOK"));
}


void packet_is_Error()
{
  uint16_t IRQStatus;
  IRQStatus = SX126XLT.readIrqStatus();                    //get the IRQ status
  Serial.print(F("SendError,"));
  Serial.print(F("Length,"));
  Serial.print(TXPacketL);
  Serial.print(F(",IRQreg,"));
  Serial.print(IRQStatus, HEX);
  SX126XLT.printIrqStatus();
  digitalWrite(LED1, LOW);                       //this leaves the LED on slightly longer for a packet error
}


bool Send_Test_Packet()
{
  uint8_t bufffersize;
    
  if (sizeof(buff) > TXBUFFER_SIZE)              //check that defined buffer is not larger than TX_BUFFER
  {
  bufffersize = TXBUFFER_SIZE;  
  }
  else
  {
   bufffersize = sizeof(buff);
  }

  TXPacketL = bufffersize;

  SX126XLT.printASCIIPacket(buff, bufffersize);
  digitalWrite(LED1, HIGH);
  if (SX126XLT.sendPacketLoRa(buff, bufffersize, 1000, TXpower, DIO1))
  {
    digitalWrite(LED1, LOW);
    return true;
  }
  else
  {
    return false;
  }
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
  SX126XLT.setDioIrqParams(IRQ_RADIO_ALL, (IRQ_TX_DONE + IRQ_RX_TX_TIMEOUT), 0, 0);   //set for IRQ on TX done and timeout on DIO1
  //the appropriate syncword can be set here, the default is LORA_MAC_PRIVATE_SYNCWORD (0x12)
  SX126XLT.setSyncWord(LORA_MAC_PRIVATE_SYNCWORD); //0x1424 written but 0x12 used
  //SX126XLT.setSyncWord(LORA_MAC_PUBLIC_SYNCWORD);  //0x3444 written but 0x34 used as needed for LoRaWAN 
}


void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(SWITCH1, INPUT_PULLUP);
  led_Flash(2, 125);

  Serial.begin(Serial_Monitor_Baud);
  Serial.println();
  Serial.print(__TIME__);
  Serial.print(F(" "));
  Serial.println(__DATE__);
  Serial.println(F(programversion));
  Serial.println();

  Serial.println(F("9_SX126XLT_LoRa_Simple_TX_and_Sleep Starting"));

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
      led_Flash(50, 50);                                            //long fast speed flash indicates device error
    }
  }

  SX126XLT.setupLoRaTX(Frequency, Offset, SpreadingFactor, Bandwidth, CodeRate, Optimisation, DEVICE_SX1262);

  Serial.print(F("Transmitter ready - TXBUFFER_SIZE "));
  Serial.println(TXBUFFER_SIZE);
  Serial.println();
}

