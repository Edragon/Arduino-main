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

char msg[10] = "ABC";



int k = 0;

void setup() {
  delay (500);
  Serial.begin(115200);
  Serial.println("Serial OK");
  delay (500);

  //// 305 bytes
  //char longMessage[350] = "I2C EEPROM library example. Split from uRTCLib\n\nThis library controls any I2C EEPROM, independent ones or incorporated on DS1307 or DS3231 RTCs.\n\nhttps://github.com/Naguissa/uEEPROMLib\nhttps://www.foroelectro.net/librerias-arduino-ide-f29/ueepromlib-arduino-libreria-simple-y-eficaz-para-e-t225.html\0";
  //int longMessageLen = strlen(longMessage);


  Wire.begin();
  //  if (!eeprom.eeprom_write(0, (byte *) longMessage, longMessageLen)) {
  //    Serial.println("Failed to store aligned STRING");
  //  } else {
  //    Serial.println("Aligned STRING correctly stored");
  //  }

}

void loop() {

  delay(5);

  // loop address
  for (int a = 0; a < 32768; a++)
  {
    //int k = random(0, 3);
    //char k = random(65, 68);  // A B C

    // write
    if ( !eeprom.eeprom_write(a, k) ) {
      Serial.println("NOK");
    } else {
      k++;
      Serial.print("address: "); Serial.print(a);

      Serial.print(" - data: "); Serial.print(k);

      Serial.println(" - OK");
    }

    if (k > 100)
    {
      k = 0;
    }

  }

}
