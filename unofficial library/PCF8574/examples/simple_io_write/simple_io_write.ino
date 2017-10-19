#include "PCF8574.h"
#include <Wire.h>

// adjust addresses if needed
PCF8574 PCF_38(0x20);  // add led to P0 (pin 4) (used as output)

void setup()
{
//your setup code here
}

void loop()
{
    //turn P0 (pin 4) of the I/O expander on and off once a second
    PCF_38.write(0, 1);
    delay(1000);
    PCF_38.write(0, 0);
    delay(1000);
}
