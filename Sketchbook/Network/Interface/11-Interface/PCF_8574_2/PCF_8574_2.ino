#include "PCF8574_2.h"

// Set i2c address
PCF8574 pcf8574(0x20);

void setup()
{
  Serial.begin(115200);

  // Set pinMode to OUTPUT
  pcf8574.pinMode(P5, OUTPUT);
  pcf8574.pinMode(P0, INPUT);
  pcf8574.begin();

  pcf8574.digitalWrite(P5, LOW);
}

void loop()
{
  
  
  int P0_read = pcf8574.digitalRead( P0 );
  Serial.println(P0_read);

  if (P0_read == 1) {
    pcf8574.digitalWrite(P5, HIGH);
    delay(200);
    pcf8574.digitalWrite(P5, LOW);
  }
}
