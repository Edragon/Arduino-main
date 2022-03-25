/*!
 * @file  useEEPROM.ino
 * @brief  This demo demonstrates how to use EEPORM of the sensor
 * @details  Save the last time and set DS1307 with the saved time after repowering on.
 * @n Or write (read) some character strings or uint8_t array to (from) EEPROM and power off to save them
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
   *  Stop RTC timing function
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
  // uint16_t setTimeBuff[7] = {5, 1, 7, 6, 9, 9, 2021};
  // DS1307.setTime(setTimeBuff);

  /**
   *  Store the current time into EEPROM, which can be used to set the time to the last saved one when powering off and restarting
   *  Call this interface when power off and call setTimeFromEEPROM() to realize when restarting
   */
  // DS1307.saveTimeToEEPROM();

  /**
   *  Set the time to the last saved one
   *    If saveTimeToEEPROM() wasn’t called before or the contents in EEPROM have been modified after the call,
   *    then time setting error may occur.
   *  If the MCU board is reset when sensor is not powered off, the interface may be miscalled and then the time may be changed by mistake.
   * Note: please set the time manually for the first time, call setTime()
   */
  DS1307.setTimeFromEEPROM();

  /**
   *  Start RTC timing function
   *  this bit is part of the seconds byte
   */
  DS1307.start();

  Serial.println("--- EEPROM Read-Write Test---");
  uint8_t someData[] = "This is data from the eeprom!";   // data to write
  /**
   *  Write EEPROM through I2C bus
   *  addr  EEPROM data address 8bits
   *  pBuf Storage and buffer for data to be written
   *  size Length of data to be written
   *  The data size that users can freely store is 247 bytes, range 0-247
   *  The last 8 bytes are for storing the time data saved when the user call saveTimeToEEPROM()
   */
  DS1307.writeEEPROM(0, someData, sizeof(someData));
  Serial.println("Written Done!");

  Serial.print("Read Data: ");
  uint8_t addr, data;
  for(addr=0; addr<sizeof(someData); addr++) 
  {
    data = 0;
    /**
     * Read EEPROM through I2C bus
     * addr  EEPROM data address 8bits
     * pBuf Storage and buffer for data to be read
     * size Length of the data to be read
     * Return the read length, returning 0 indicates reading failed
     */
    DS1307.readEEPROM(addr, &data, sizeof(data));
    Serial.print((char)data);
  }
  Serial.println();
}

void loop()
{
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

  /**
   *  Store the current time into EEPROM, which is used to set the time to the last saved one after powering off and restarting
   *  Call the interface when powering off and call setTimeFromEEPROM() to realize when restarting
   */
  DS1307.saveTimeToEEPROM();

 delay(1000);
}
