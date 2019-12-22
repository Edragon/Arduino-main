/*
******************************************************************************************************

  LoRaTracker Programs for Arduino

  Copyright of the author Stuart Robinson - 24/10/19

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

  This program checks that an SX126X LoRa device can be accessed. There should be two short LED flashes
  at startup. If the SX126X is detected there will be two more LED flashes and the contents of the
  registers from 0x900 to 0x9F are printed, this is a copy of a typical printout below.

  If the SX126X is not detected the LED will flashes rapidly.

  1_SX126XLT_LoRa_Register_Test Starting
Device found
Frequency at reset 915000000
Reg    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
0x680  04 C0 00 40 00 00 00 00 00 00 00 00 00 00 00 00 
0x690  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x6A0  00 02 41 55 00 14 7B 08 20 0A 20 07 00 00 00 00 
0x6B0  00 00 00 00 04 00 14 20 21 00 C6 0F 1D 0F 10 21 
0x6C0  97 23 52 25 56 53 65 64 00 00 00 00 00 00 00 00 
0x6D0  00 0C 03 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x6E0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x6F0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x700  00 00 0C 57 02 04 00 00 00 00 00 00 00 00 00 00 
0x710  00 00 00 00 00 00 00 00 00 7F 28 00 00 FF 00 01 
0x720  37 2A 53 00 80 00 40 00 00 0A 0A 00 00 00 00 00 
0x730  24 00 21 28 B0 30 0D 1A 09 70 00 08 58 0B 19 0A 
0x740  14 24 6A 96 00 18 00 00 00 00 00 00 00 00 00 00 
0x750  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x760  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x770  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x780  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x790  00 00 00 00 00 00 00 28 4C 09 37 00 00 00 00 00 
0x7A0  0E 03 00 3C 00 05 01 56 00 00 00 00 00 00 00 00 
0x7B0  02 03 02 03 05 08 06 04 02 FE FC FD 03 0B 13 0A 
0x7C0  00 3C 06 19 2A 07 61 09 02 04 21 28 02 00 00 00 
0x7D0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x7E0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x7F0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x800  00 00 00 00 01 07 20 1E 00 10 19 04 0F FF 0F FF 
0x810  10 00 10 00 10 00 10 00 00 00 00 00 00 00 00 00 
0x820  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x830  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x840  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x850  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x860  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x870  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x880  03 00 00 5F 10 08 00 00 08 05 00 39 30 00 00 0C 
0x890  00 00 00 00 00 0F 0A 07 10 00 26 01 01 53 06 07 
0x8A0  10 00 AA 20 5A 04 F0 00 56 56 54 43 94 20 40 00 
0x8B0  00 83 11 00 01 04 0A 4C 14 0A 2F 01 6B FF FF 00 
0x8C0  00 A0 20 00 00 00 AC 00 1C 00 00 AB 05 30 00 00 
0x8D0  0C 14 14 40 06 00 00 10 C8 00 00 00 00 00 31 39 
0x8E0  90 39 0C 04 40 20 1C 18 03 00 05 04 03 02 01 01 
0x8F0  00 00 00 00 30 00 00 00 00 00 00 00 00 00 00 00 
0x900  30 00 00 00 00 00 00 00 00 00 00 00 24 04 47 04 
0x910  14 12 12 04 00 03 0A 00 15 35 09 00 02 1F 5F 08 
0x920  01 04 05 00 00 00 00 00 00 00 00 00 00 00 00 00 
Changed Frequency 434400000
******************************************************************************************************
*/

//These are the pin definitions for one of the LoRaTracker boards, be sure to change them to match
//your own setup.

#define LED1 8                                  //for on board LED
#define RFBUSY 3                                //SX126X RFBUSY pin  
#define NSS 10                                  //SX126X device select
#define NRESET 9                                //SX126X reset pin
#define DIO1 -1                                 //not used 
#define DIO2 -1                                 //not used
#define DIO3 -1                                 //not used

#define programversion "V1.0"
#define Serial_Monitor_Baud 115200              //serial monitor baud rate 

#include <SPI.h>
#include <SX126XLT.h>

SX126XClass SX126XLT;

void loop()
{
  uint32_t frequency;
  frequency = SX126XLT.getFreqInt();
  Serial.print(F("Frequency at reset "));
  Serial.println(frequency);

  digitalWrite(LED1, HIGH);
  SX126XLT.printRegisters(0x680, 0x920);
  SX126XLT.setRfFrequency(434400000, 0);
  frequency = SX126XLT.getFreqInt();
  Serial.print(F("Changed Frequency "));
  Serial.println(frequency);
  Serial.println();
  Serial.println();
  digitalWrite(LED1, LOW);
  delay(5000);
  SX126XLT.resetDevice();
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


void setup()
{
  pinMode(LED1, OUTPUT);
  led_Flash(2, 125);

  Serial.begin(Serial_Monitor_Baud);
  Serial.println();
  Serial.print(__TIME__);
  Serial.print(F(" "));
  Serial.println(__DATE__);
  Serial.println(F(programversion));
  Serial.println();

  Serial.println(F("1_SX126XLT_LoRa_Register_Test Starting"));

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

}

