# M2M_LM75A

Arduino library for the LM75A temperature sensor by NXP and Texas Instruments.

Written by Jonny Bergdahl for M2M Solutions AB. 

# License

Copyright (c) 2016-2017, M2M Solutions AB. All rights reserved.

This library is open source. It is released under the [MIT License](LICENSE.txt). 
Please see the included LICENSE.txt file for more information.

# LM75A

 LM75A is a temperature sensor communicating over the I2C protocol.

The NXP version of LM75A has 11 significant bits of the temperature, which gives a 
resolution of 0.125 °C. The standard version only uses 9 bits, which gives a resuolution 
of 0.5 °C.

I2C address span is 0x48 - 0x4F, and is set with pins A0/A1/A2.

Power up mode:
 - Comparator mode
 - Tos = 80 °C
 - Thyst = 75 °C
 - OS active low

Pin 3 is the OS (overtemperature shutdown). It can be used to to trigger a pin interrupt.
Set the functional mode, Tos and Thyst if you want to use this.

## Resources
LM75A data sheets.
 - [NXP LM75A data sheet](http://www.nxp.com/documents/data_sheet/LM75A.pdf)
 - [Texas Instruments data sheet](http://www.ti.com/lit/ds/symlink/lm75a.pdf)
