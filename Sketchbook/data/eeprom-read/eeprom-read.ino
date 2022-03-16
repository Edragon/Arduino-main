/**
   I2C EEPROM library example. Split from uRTCLib https://github.com/Naguissa/uRTCLib

   This library controls any I2C EEPROM, independent ones or incorporated on DS1307 or DS3231 RTCs.

   @copyright Naguissa
   @author Naguissa
   @email naguissa@foroelectro.net
   @url https://www.foroelectro.net/librerias-arduino-ide-f29/ueepromlib-arduino-libreria-simple-y-eficaz-para-e-t225.html
   @url https://github.com/Naguissa/uEEPROMLib
   @version 1.2.0
*/
#include "Arduino.h"
#include "Wire.h"
#include "uEEPROMLib.h"


// uEEPROMLib eeprom;
uEEPROMLib eeprom(0x50);



char k[10] = "";

int b = 0;

void setup() {
  delay (500);
  Serial.begin(115200);
  Serial.println("Serial OK");
  delay (500);

  Wire.begin();


}

void loop() {
  //delay(10);

  for (int a = 0; a < 32768; a++)
  {
    //if ( !eeprom.eeprom_read(a, (byte *) k, 1) ) {
    b = eeprom.eeprom_read(a);
    Serial.print("address: "); Serial.print(a);
    Serial.print(" - data: "); Serial.print(b);
    Serial.println(" - ROK");
  }
}
