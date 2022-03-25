/*!
 * @file  setTimeAndSqw.ino
 * @brief  Set time and start timing, set pin sqw output
 * @details  Read the time and pin sqw mode
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2021-11-18
 * @url  https://github.com/DFRobot/DFRobot_DS1307
 */
#include <DFRobot_DS1307.h>


/* Constructor */
DFRobot_DS1307 DS1307;

void setup()
{
  Serial.begin(115200);

  // Initialize sensor
  while( !(DS1307.begin()) ){
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  /**
   *  According to the set time type, input the value of the corresponding range, set time
   *  type Time type to be obtained, eTimeType_t:
   *    eSEC, eMIN, eHR, eDOW, eDATE, eMTH, eYR
   *  typeTime According to the selected time type, write the value of the corresponding range
   *    When setting eSEC type, range: 00-59
   *    When setting eMIN type, range: 00-59
   *    When setting eHR type, range: 00-23
   *    When setting eDOW type, range: 01-07
   *    When setting eDATE type, range: 01-31
   *    When setting eMTH type, range: 01-12
   *    When setting eYR type, range: 2000-2099
   *  Note: Values out of range will result in a setting error
   */
  DS1307.setTypeTime(DS1307.eYR, 2000);

  /**
   *  Get the time of one type from rtc module and convert it to uint16_t
   *  type Time type to be obtained, eTimeType_t:
   *    eSEC, eMIN, eHR, eDOW, eDATE, eMTH, eYR
   *  According to the selected time type, return the value uint16_t of the corresponding range
   *    When reading eSEC type, range: 00-59
   *    When reading eMIN type, range: 00-59
   *    When reading eHR type, range: 00-23
   *    When reading eDOW type, range: 01-07
   *    When reading eDATE type, range: 01-31
   *    When reading eMTH type, range: 01-12
   *    When reading eYR type, range: 2000-2099
   */
  Serial.print("get type time: ");
  Serial.println(DS1307.getTypeTime(DS1307.eYR));

  /**
   *  Stop RTC timer function
   *  this bit is part of the seconds byte
   */
  DS1307.stop();
  /**
   *  According to the available array, set all the time
   *  setTimeBuff Array in the following format (type is uint16_t)
   *    setTimeBuff[0] for eSEC type, range: 00-59
   *    setTimeBuff[1] for eMIN type, range: 00-59
   *    setTimeBuff[2] for eHR type, range: 00-23
   *    setTimeBuff[3] for eDOW type, range: 01-07
   *    setTimeBuff[4] for eDATE type, range: 01-31
   *    setTimeBuff[5] for eMTH type, range: 01-12
   *    setTimeBuff[6] for eYR type, range: 2000-2099
   *  Note: Values out of range will result in a setting error
   */
  uint16_t setTimeBuff[7] = {5, 1, 7, 6, 9, 9, 2021};
  DS1307.setTime(setTimeBuff);
  /**
   *  Start RTC timing function
   *  this bit is part of the seconds byte
   */
  DS1307.start();

  /**
   *  control the operation of the SQW/OUT pin
   *  mode SQW Pin output mode:
   *    eSquareWave_LOW, eSquareWave_HIGH, eSquareWave_1Hz, 
   *    eSquareWave_4kHz, eSquareWave_8kHz, eSquareWave_32kHz
   */
  DS1307.setSqwPinMode(DS1307.eSquareWave_1Hz);

}

void loop()
{
  /**
   *  Get the current output mode of SQW/OUT pin
   *  eSqwPinMode_t:
   *    eSquareWave_LOW, eSquareWave_HIGH, eSquareWave_1Hz, 
   *    eSquareWave_4kHz, eSquareWave_8kHz, eSquareWave_32kHz
   */
  if(DS1307.eSquareWave_1Hz == DS1307.getSqwPinMode()){
    Serial.print("SQW/OUT pin: 1Hz | ");
  }

  /**
   *  Get the time from rtc module and convert it to uint16_t
   *  getTimeBuff Array for buffering the obtained time, uint16_t *
   *    getTimeBuff[0] for eSEC type, range: 00-59
   *    getTimeBuff[1] for eMIN type, range: 00-59
   *    getTimeBuff[2] for eHR type, range: 00-23
   *    getTimeBuff[3] for eDOW type, range: 01-07
   *    getTimeBuff[4] for eDATE type, range: 01-31
   *    getTimeBuff[5] for eMTH type, range: 01-12
   *    getTimeBuff[6] for eYR type, range: 2000-2099
   */
  uint16_t getTimeBuff[7] = {0};
  DS1307.getTime(getTimeBuff);
  char outputarr[128];
  sprintf(outputarr, "time: %d/%d/%d-%d %d:%d:%d\r\n",
            getTimeBuff[6],
            getTimeBuff[5],
            getTimeBuff[4],
            getTimeBuff[3],
            getTimeBuff[2],
            getTimeBuff[1],
            getTimeBuff[0]
            );
  Serial.print(outputarr);

 delay(1000);
}
