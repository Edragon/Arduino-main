#include "Wire.h"

#define PCF8591 (0x48)

byte adcvalue0, adcvalue1, adcvalue2, adcvalue3;
 
void setup()
{
 Wire.begin();
 Serial.begin(9600);
}
 
void loop()
{
 Wire.beginTransmission(PCF8591);
 Wire.write(0x04);
 Wire.endTransmission();
 Wire.requestFrom(PCF8591, 5);
 
 adcvalue0=Wire.read();
 adcvalue1=Wire.read();
 adcvalue2=Wire.read();
 adcvalue3=Wire.read();
 
 Serial.print(adcvalue0);
 Serial.print(" ,");
 Serial.print(adcvalue1);  // sensor
 Serial.print(" ,");
 Serial.print(adcvalue2); 
 Serial.print(" ,");
 Serial.print(adcvalue3);  // Trim pot
 Serial.println();
 
 delay(1000);
}
