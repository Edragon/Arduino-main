

#include <SPIFlash.h>    //get it here: https://github.com/LowPowerLab/SPIFlash
#include <SPI.h>

#define SERIAL_BAUD      115200
char input = 0;
long lastPeriod = -1;

#ifdef __AVR_ATmega1284P__
  #define LED           15 // Moteino MEGAs have LEDs on D15
  #define FLASH_SS      10 // and FLASH SS on D23
#else
  #define LED           13 // Moteinos have LEDs on D9
  #define FLASH_SS      10 // and FLASH SS on D8
#endif

//////////////////////////////////////////
// flash(SPI_CS, MANUFACTURER_ID)
// SPI_CS          - CS pin attached to SPI flash chip (8 in case of Moteino)
// MANUFACTURER_ID - OPTIONAL, 0x1F44 for adesto(ex atmel) 4mbit flash
//                             0xEF30 for windbond 4mbit flash
//////////////////////////////////////////
SPIFlash flash(FLASH_SS, 0xEF40);

void setup(){
  Serial.begin(SERIAL_BAUD);
  Serial.print("Start...");

  if (flash.initialize())
  {
    Serial.println("Init OK!");
    Blink(LED, 20, 10);
  }
  else
    Serial.println("Init FAIL!");
  
  delay(1000);
}

void loop(){
  // Handle serial input (to allow basic DEBUGGING of FLASH chip)
  // ie: display first 256 bytes in FLASH, erase chip, write bytes at first 10 positions, etc
  if (Serial.available() > 0) {
    input = Serial.read();
    if (input == 'd') //d=dump flash area
    {
      Serial.println("Flash content:");
      int counter = 0;

      while(counter<=256){
        Serial.print(flash.readByte(counter++), HEX);
        Serial.print('.');
      }
      
      Serial.println();
    }
    else if (input == 'e')
    {
      Serial.print("Erasing Flash chip ... ");
      flash.chipErase();
      while(flash.busy());
      Serial.println("DONE");
    }
    else if (input == 'i')
    {
      Serial.print("DeviceID: ");
      Serial.println(flash.readDeviceId(), HEX);
    }
    else if (input >= 48 && input <= 57) //0-9
    {
      Serial.print("\nWriteByte("); Serial.print(input); Serial.print(")");
      flash.writeByte(input-48, millis()%2 ? 0xaa : 0xbb);
    }
  }

  // Periodically blink the onboard LED while listening for serial commands
  if ((int)(millis()/500) > lastPeriod)
  {
    lastPeriod++;
    pinMode(LED, OUTPUT);
    digitalWrite(LED, lastPeriod%2);
  }
}

void Blink(byte PIN, int DELAY_MS, byte loops)
{
  pinMode(PIN, OUTPUT);
  while (loops--)
  {
    digitalWrite(PIN,HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN,LOW);
    delay(DELAY_MS);  
  }
}
