# TLC5615 Arduino driver

Created & maintained by Max Chan.

## Introduction

This driver interfaces the microcontroller with the Texas Instruments TLC5615
SPI DAC using the microcontroller's built-in SPI hardware.

## Using the Driver

The driver uses a similar syntax as the default PWM outputs, albeit with an
object.

```Arduino
#include <TLC5615.h>

TLC5615 dac(10);

void setup(void)
{
  dac.begin();
}

void loop(void)
{
  dac.analogWrite(millis());
}
```

## License

This library is licensed under the 3-clause BSD license.
