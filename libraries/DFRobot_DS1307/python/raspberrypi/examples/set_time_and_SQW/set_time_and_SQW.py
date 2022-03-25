# -*- coding: utf-8 -*
'''!
  @file  set_time_and_SQW.py
  @brief  Set time and start timing, set pin sqw output
  @details  Read the time and pin sqw mode
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
    # According to the set time type, input the value of the corresponding range, set time
    # type Time type to be obtained:
    #   e_SEC, e_MIN, e_HR, e_DOW, e_DATE, e_MTH, e_YR
    # type_time According to the selected time type, write the value of the corresponding range
    #   When reading e_SEC type, range: 00-59
    #   When reading e_MIN type, range: 00-59
    #   When reading e_HR type, range: 00-23
    #   When reading e_DOW type, range: 01-07
    #   When reading e_DATE type, range: 01-31
    #   When reading e_MTH type, range: 01-12
    #   When reading e_YR type, range: 2000-2099
  '''
  DS1307.set_type_time(DS1307.e_YR, 2000)

  '''
    # Get the time of one type from rtc module
    # type Time type to be obtained:
    #   e_SEC, e_MIN, e_HR, e_DOW, e_DATE, e_MTH, e_YR
    # According to the selected time type, return the value of the corresponding range
    #   When reading e_SEC type, range: 00-59
    #   When reading e_MIN type, range: 00-59
    #   When reading e_HR type, range: 00-23
    #   When reading e_DOW type, range: 01-07
    #   When reading e_DATE type, range: 01-31
    #   When reading e_MTH type, range: 01-12
    #   When reading e_YR type, range: 2000-2099
  '''
  print("get type time:  %u" %( DS1307.get_type_time(DS1307.e_YR) ) )

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
    #   Note: Values out of range will result in a setting error
  '''
  time_list = [5, 1, 7, 6, 9, 9, 2021]
  DS1307.set_time(time_list)

  '''
    # Start RTC timing function
    # this bit is part of the seconds byte
  '''
  DS1307.start

  '''
    # Control the operation of the SQW/OUT pin
    # mode SQW Pin output mode:
    #   e_square_wave_LOW, e_square_wave_HIGH, e_square_wave_1Hz, 
    #   e_square_wave_4kHz, e_square_wave_8kHz, e_square_wave_32kHz
  '''
  DS1307.set_SQW_pin_mode(DS1307.e_square_wave_1Hz)


def loop():
  '''
    # Get the current output mode of SQW/OUT pin
    # Output mode:
    #   e_square_wave_LOW, e_square_wave_HIGH, e_square_wave_1Hz, 
    #   e_square_wave_4kHz, e_square_wave_8kHz, e_square_wave_32kHz
  '''
  if DS1307.e_square_wave_1Hz == DS1307.get_SQW_pin_mode():
    print("SQW/OUT pin: 1Hz | ", end='')

  '''
    # Get the time from rtc module
    # The obtained time list
    #   time_list[0] for e_SEC type, range: 00-59
    #   time_list[1] for e_MIN type, range: 00-59
    #   time_list[2] for e_HR type, range: 00-23
    #   time_list[3] for e_DOW type, range: 01-07
    #   time_list[4] for e_DATE type, range: 01-31
    #   time_list[5] for e_MTH type, range: 01-12
    #   time_list[6] for e_YR type, range: 2000-2099
  '''
  time_list = DS1307.get_time()
  print("time: %u/%u/%u-%u %u:%u:%u \n" %( time_list[6], time_list[5],
  time_list[4], time_list[3], time_list[2], time_list[1], time_list[0] ))

  time.sleep(1)


if __name__ == "__main__":
  setup()
  while True:
    loop()
