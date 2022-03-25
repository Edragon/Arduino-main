# -*- coding: utf-8 -*
'''!
  @file  use_EEPROM.py
  @brief  This demo demonstrates how to use EEPORM of the sensor
  @details  Save the last time and set DS1307 with the saved time after repowering on.
  @n Or write (read) some character strings or uint8_t array to (from) EEPROM and power off to save them.
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license  The MIT License (MIT)
  @author  [qsjhyy](yihuan.huang@dfrobot.com)
  @version  V1.0
  @date  2021-12-02
  @url  https://github.com/DFRobot/DFRobot_DS1307
'''
from __future__ import print_function
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))))

from DFRobot_DS1307 import *

'''
  # Module I2C communication init
  # ds1307_addr I2C communication address
  # bus I2C bus
'''
DS1307 = DFRobot_DS1307()


def setup():
  while (DS1307.begin() == False):
    print ('Please check that the device is properly connected')
    time.sleep(3)
  print("DS1307 begin successfully!!!")

  '''
    # Stop RTC timing function
    # this bit is part of the seconds byte
  '''
  DS1307.stop

  '''
    # According to the available array, set all the time
    # time_list Array in the following format
    #   time_list[0] for e_SEC type, range: 00-59
    #   time_list[1] for e_MIN type, range: 00-59
    #   time_list[2] for e_HR type, range: 00-23
    #   time_list[3] for e_DOW type, range: 01-07
    #   time_list[4] for e_DATE type, range: 01-31
    #   time_list[5] for e_MTH type, range: 01-12
    #   time_list[6] for e_YR type, range: 2000-2099
    #   Note: out of range will result in a setting error
  '''
  # time_list = [5, 1, 7, 6, 9, 9, 2021]
  # DS1307.set_time(time_list)

  '''
    # Store the current time into EEPROM, which can be used to set the time to the last saved one after powering off and restarting
    # Call the interface when powering off and call setTimeFromEEPROM() to realize when restarting
  '''
  # DS1307.save_time_to_EEPROM

  '''
    # Set the time to the last saved one
    # If saveTimeToEEPROM() wasn’t called before or the contents in EEPROM have been modified after the call,
    # the time setting error may occur.
    # If the MCU board is reset when sensor is not powered off, the interface may be miscalled and then the time may be changed by mistake.
  '''
  DS1307.set_time_from_EEPROM

  '''
    # Start RTC timing function
    # this bit is part of the seconds byte
  '''
  DS1307.start

  print("--- EEPROM Read-Write Test---")
  str_data = "This is data from the eeprom!"   # data to write
  list_data = list(str_data)   # Character string converting list
  '''
    # writes data to a EEPROM
    # reg EEPROM address
    # data written data
    # The data size that users can freely store is 247 bytes, range 0-247
    # The last 8 bytes are for storing the time data saved when the user call saveTimeToEEPROM()
  '''
  # Convert the character elements in the character string list to their corresponding integers one by one, and write them to EEPROM.
  DS1307.write_EEPROM( 0, list(map( ord, list_data )) )
  print("Written Done!")

  print("Read Data: ", end='')
  '''
    # read the data from the EEPROM
    # reg EEPROM address
    # length read data length
    # read data list
  '''
  # Convert the integer elements in the integer list read from EEPROM to their corresponding characters, and then convert the obtained character list to the string for printing.
  print(''.join( list(map( chr, DS1307.read_EEPROM(0, len(list_data)) )) ))

def loop():
  '''
    # Get the time from rtc module
    # The obtained time list
    #   rtc[0] for e_SEC type, range: 00-59
    #   rtc[1] for e_MIN type, range: 00-59
    #   rtc[2] for e_HR type, range: 00-23
    #   rtc[3] for e_DOW type, range: 01-07
    #   rtc[4] for e_DATE type, range: 01-31
    #   rtc[5] for e_MTH type, range: 01-12
    #   rtc[6] for e_YR type, range: 2000-2099
  '''
  time_list = DS1307.get_time()
  print("time: %u/%u/%u-%u %u:%u:%u \n" %(time_list[6], time_list[5], 
  time_list[4], time_list[3], time_list[2], time_list[1], time_list[0]))

  '''
    # Store the current time into EEPROM, which is used to set the time to the last saved one after power off
    # Call the interface when powering off and call setTimeFromEEPROM() to realize when restarting
  '''
  DS1307.save_time_to_EEPROM

  time.sleep(1)


if __name__ == "__main__":
  setup()
  while True:
    loop()
