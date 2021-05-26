[![Arduino CI](https://github.com/argandas/serialEEPROM/workflows/Arduino_CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/I2C_EEPROM/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/v/release/argandas/serialeeprom)](https://github.com/argandas/serialEEPROM/releases)

# serialEEPROM

Arduino Library for I2C EEPROM


<!-- START COMPATIBILITY TABLE -->

## Compatibility

Device      | Tested Works | Doesn't Work | Not Tested | Size (Bytes) | Page Size (Bytes) | Address range 
----------- | :----------: | :----------: | :--------: | :----------: | :---------------: | :------------
AT24C256    |       X      |              |            | 32768        | 64                | 0x50 ~ 0x53
AT24C128    |       X      |              |            | 16384        | 64                | 0x50 ~ 0x53
24C01C      |       X      |              |            | 128          | 16                | 0x50 ~ 0x57

  * [AT24C128/256](http://www.atmel.com/Images/doc0670.pdf): Atmel 128K/256K I2C EEPROM (16KB/32KB)
  * [24C01C](http://ww1.microchip.com/downloads/en/DeviceDoc/21201K.pdf): Microchip 1K I2C EEPROM (128B)

<!-- END COMPATIBILITY TABLE -->

## Usage

For example if you are using the Microchip's 24C01C Serial EEPROM, you should define a serialEEPROM object as follow:
```c++
serialEEPROM myEEPROM(0x50, 128, 16);
```
Where "0x50" is the device address, 128 is the memory size in bytes and 16 is the page address in bytes.

For more information about how to use this library please take a look on the examples folder.
