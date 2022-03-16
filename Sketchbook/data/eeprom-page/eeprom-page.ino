
#include "Arduino.h"
#include "Wire.h"
#include "uEEPROMLib.h"


// uEEPROMLib eeprom;
uEEPROMLib eeprom(0x50);

int buck[64];

void setup() {
  delay (500);
  Serial.begin(115200);
  Serial.println("Serial OK");
  delay (500);
  Wire.begin();

  for (int i =0; i <64; i++) {
    buck[i] = i;
    }


}

void loop() {
//  for (int i =0; i <64; i++) {
//    Serial.println(buck[i]);
//    }
//    delay(1000);
    
  
  for (int k = 0; k< 513; k++){
    if (  !eeprom.eeprom_write(k * 64, (byte *) buck, 64)  ) {
        Serial.println("Failed to write to EEPROM!");
      } else {
        Serial.print("ADDRESS: "); Serial.print(k * 64);
        Serial.print("DATA: "); Serial.print(buck[0]);
        Serial.println("");
        }
  }
//      
}
