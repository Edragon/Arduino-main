# DFRobot_DS1307
* [中文版](./README_CN.md)

The DS1307 serial real-time clock (RTC) is a low-power, full binary-coded decimal (BCD) clock/calendar 
plus 56 bytes of NV SRAM. Address and data are transferred serially through an I2C, bidirectional bus. 
The clock/calendar provides seconds, minutes, hours, day, date, month, and year information. The end of 
the month date is automatically adjusted for months with fewer than 31 days, including corrections for leap 
year. The clock operates in either the 24-hour or 12-hour format with AM/PM indicator. The DS1307 has a 
built-in power-sense circuit that detects power failures and automatically switches to the backup supply. 
Timekeeping operation continues while the part operates from the backup supply.

![产品实物图](../../resources/images/DS1307.png)


## Product Link (https://www.dfrobot.com/product-879.html)
    SKU: DFR0151


## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)


## Summary

* Real-Time Clock (RTC) Counts Seconds, Minutes, Hours, Date of the Month, Month, Day of the week, and Year with Leap-Year Compensation Valid Up to 2100
* 56-Byte, Battery-Backed, General-Purpose RAMwith Unlimited Writes
* I2C Serial Interface
* Programmable Square-Wave Output Signal
* Automatic Power-Fail Detect and Switch Circuitry
* Consumes Less than 500nA in Battery-Backup Mode with Oscillator Running
* Optional Industrial Temperature Range:-40°C to +85°C 
* Available in 8-Pin Plastic DIP or SO
* Underwriters Laboratories (UL) Recognized


## Installation

To use the library, first download the library file, paste it into the directory you specified, then open the Examples folder and run the demo in that folder.


## Methods

```python

  '''!
    @brief Init function
    @return  Return init status
    @retval True indicate init succeed
    @retval False indicate init failed
  '''
  def begin(self):

  '''!
    @brief Get the time from rtc module
    @return The obtained time list
    @n rtc[0] for e_SEC type, range: 00-59
    @n rtc[1] for e_MIN type, range: 00-59
    @n rtc[2] for e_HR type, range: 00-23
    @n rtc[3] for e_DOW type, range: 01-07
    @n rtc[4] for e_DATE type, range: 01-31
    @n rtc[5] for e_MTH type, range: 01-12
    @n rtc[6] for e_YR type, range: 2000-2099
  '''
  def get_time(self):

  '''!
    @brief Get the time of one type from rtc module
    @param type Time type to be obtained:
    @n e_SEC, e_MIN, e_HR, e_DOW, e_DATE, e_MTH, e_YR
    @return According to the selected time type, return the value of the corresponding range
    @n When reading e_SEC type, range: 00-59
    @n When reading e_MIN type, range: 00-59
    @n When reading e_HR type, range: 00-23
    @n When reading e_DOW type, range: 01-07
    @n When reading e_DATE type, range: 01-31
    @n When reading e_MTH type, range: 01-12
    @n When reading e_YR type, range: 2000-2099
  '''
  def get_type_time(self, type):

  '''!
    @brief According to the available array, set all the time
    @param rtc Array in the following format
    @n rtc[0] for e_SEC type, range: 00-59
    @n rtc[1] for e_MIN type, range: 00-59
    @n rtc[2] for e_HR type, range: 00-23
    @n rtc[3] for e_DOW type, range: 01-07
    @n rtc[4] for e_DATE type, range: 01-31
    @n rtc[5] for e_MTH type, range: 01-12
    @n rtc[6] for e_YR type, range: 2000-2099
    @n Note: Values out of range will result in a setting error
  '''
  def set_time(self, rtc):

  '''!
    @brief According to the set time type, input the value of corresponding range, set time
    @param type Time type to be obtained:
    @n e_SEC, e_MIN, e_HR, e_DOW, e_DATE, e_MTH, e_YR
    @param type_time According to the selected time type, write the value of the corresponding range
    @n When reading e_SEC type, range: 00-59
    @n When reading e_MIN type, range: 00-59
    @n When reading e_HR type, range: 00-23
    @n When reading e_DOW type, range: 01-07
    @n When reading e_DATE type, range: 01-31
    @n When reading e_MTH type, range: 01-12
    @n When reading e_YR type, range: 2000-2099
  '''
  def set_type_time(self, type, type_time):

  '''!
    @brief Start RTC timing function
    @n this bit is part of the seconds byte
  '''
  @property
  def start(self):

  '''!
    @brief Stop RTC timing function
    @n this bit is part of the seconds byte
  '''
  @property
  def stop(self):

  '''!
    @brief Control the operation of the SQW/OUT pin
    @param mode SQW Pin output mode:
    @n e_square_wave_LOW, e_square_wave_HIGH, e_square_wave_1Hz, 
    @n e_square_wave_4kHz, e_square_wave_8kHz, e_square_wave_32kHz
  '''
  def set_SQW_pin_mode(self, mode):

  '''!
    @brief Get the current output mode of SQW/OUT pin
    @return output mode:
    @n e_square_wave_LOW, e_square_wave_HIGH, e_square_wave_1Hz, 
    @n e_square_wave_4kHz, e_square_wave_8kHz, e_square_wave_32kHz
  '''
  def get_SQW_pin_mode(self):

  '''!
    @brief Store the current time into EEPROM, which can be used to set the time to the last saved one after power off
    @n Call the interface when powering off and call setTimeFromEEPROM() to realize when restarting
  '''
  @property
  def save_time_to_EEPROM(self):

  '''!
    @brief Set the time to the last saved one
    @n If saveTimeToEEPROM() wasn’t called before or the contents in EEPROM have been modified after the call,
    @n the time setting error may occur.
    @note If the MCU board is reset when sensor is not powered off, the interface may be miscalled and the time may be changed by mistake.
  '''
  @property
  def set_time_from_EEPROM(self):

  '''!
    @brief Writes data to a EEPROM
    @param reg EEPROM address
    @param data written data
  '''
  def write_EEPROM(self, reg, data):

  '''!
    @brief Read the data from the EEPROM
    @param reg EEPROM address
    @param length read data length
    @return read data list
  '''
  def read_EEPROM(self, reg, length):

```


## Compatibility

* RaspberryPi Version

| Board        | Work Well | Work Wrong | Untested | Remarks |
| ------------ | :-------: | :--------: | :------: | ------- |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |           |            |    √     |         |
| RaspberryPi4 |     √     |            |          |         |

* Python Version

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | ------- |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |


## History

- 2021/12/02 - Version 1.0.0 released.


## Credits

Written by qsjhyy(yihuan.huang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))

