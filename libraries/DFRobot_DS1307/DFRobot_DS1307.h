/*!
 * @file  DFRobot_DS1307.h
 * @brief  Define infrastructure of DFRobot_DS1307 class
 * @details  The clock/calendar provides seconds, minutes, hours, 
 * @n  day, date, month, and year information.
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2021-11-18
 * @url  https://github.com/DFRobot/DFRobot_DS1307
 */
#ifndef __DFRobot_DS1307_H__
#define __DFRobot_DS1307_H__

// include types & constants of Wiring core API
#include <Arduino.h>
#include <Wire.h>


// #define ENABLE_DBG   //!< Open the macro and you can see the details of the program
#ifdef ENABLE_DBG
  #define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
  #define DBG(...)
#endif

// DS1307 I2C address
#define EEPROM_I2C_ADDR   uint8_t(0x50)   ///< EEPROM I2C address
#define DS1307_I2C_ADDR   uint8_t(0x68)   ///< DS1307 I2C address
#define DS1307_BASE_YR            2000    ///< Base year is 2000

// DS1307 register address
#define DS1307_SEC_REG    uint8_t(0x00)   ///< DS1307 Seconds register address
#define DS1307_MIN_REG    uint8_t(0x01)   ///< DS1307 Minutes register address
#define DS1307_HR_REG     uint8_t(0x02)   ///< DS1307 Hour register address
#define DS1307_DOW_REG    uint8_t(0x03)   ///< DS1307 DAY register address
#define DS1307_DATE_REG   uint8_t(0x04)   ///< DS1307 Date register address
#define DS1307_MTH_REG    uint8_t(0x05)   ///< DS1307 Month register address
#define DS1307_YR_REG     uint8_t(0x06)   ///< DS1307 Year register address
#define DS1307_CTL_REG    uint8_t(0x07)   ///< DS1307 Control register address
#define DS1307_RAM_REG    uint8_t(0x08)   ///< DS1307 RAM register start address(08h–3Fh), size: 56 x 8bits

// Define register bit masks
#define DS1307_CLOCKHALT B10000000

#define DS1307_LO_BCD    B00001111
#define DS1307_HI_BCD    B11110000

#define DS1307_SEC_MASK    B01111111
#define DS1307_MIN_MASK    B01111111
#define DS1307_HR_MASK     B00111111
#define DS1307_DOW_MASK    B00000111
#define DS1307_DATE_MASK   B00111111
#define DS1307_MTH_MASK    B00011111
#define DS1307_YR_MASK     B11111111


class DFRobot_DS1307
{
public:

  /**
   * @enum  eTimeType_t
   * @brief  Time type select
   */
  typedef enum
  {
    eSEC  = 0,
    eMIN,
    eHR,
    eDOW,
    eDATE,
    eMTH,
    eYR,
  }eTimeType_t;

  /**
   * @enum  eSqwPinMode_t
   * @brief  Set the output square wave mode
   */
  typedef enum
  {
    eSquareWave_LOW   = 0x00,   /**< Not output square wave, output low level */
    eSquareWave_HIGH  = 0x80,   /**< Not output square wave, output HIGH level */
    eSquareWave_1Hz   = 0x10,   /**< 1Hz square wave */
    eSquareWave_4kHz  = 0x11,   /**< 4.096kHz square wave */
    eSquareWave_8kHz  = 0x12,   /**< 8.192kHz square wave */
    eSquareWave_32kHz = 0x13,   /**< 32.768kHz square wave */
  }eSqwPinMode_t;

public:

  /**
   * @fn DFRobot_DS1307
   * @brief Constructor
   * @param pWire Wire object is defined in Wire.h, so just use &Wire, and the methods in Wire can be pointed to and used
   * @param ds1307Addr I2C communication address of DS1307
   * @return None
   */
  DFRobot_DS1307(TwoWire *pWire=&Wire, uint8_t ds1307Addr=DS1307_I2C_ADDR);

  /**
   * @fn begin
   * @brief Init function
   * @return true indicate init succeed; false indicate init failed
   */
  bool begin(void);

  /**
   * @fn getTime
   * @brief Get the time from rtc module and convert it to uint16_t
   * @param rtc Array for buffering the obtained time *
   * @n rtc[0] for eSEC type, range: 00-59
   * @n rtc[1] for eMIN type, range: 00-59
   * @n rtc[2] for eHR type, range: 00-23
   * @n rtc[3] for eDOW type, range: 01-07
   * @n rtc[4] for eDATE type, range: 01-31
   * @n rtc[5] for eMTH type, range: 01-12
   * @n rtc[6] for eYR type, range: 2000-2099
   * @return None
   */
  void getTime(uint16_t *rtc);

  /**
   * @fn getTypeTime
   * @brief Get the time of one type from rtc module and convert it to uint16_t
   * @param type Time type to be obtained, eTimeType_t:
   * @n eSEC, eMIN, eHR, eDOW, eDATE, eMTH, eYR
   * @return According to the selected time type, return the value uint16_t of the corresponding range
   * @n When reading eSEC type, range: 00-59
   * @n When reading eMIN type, range: 00-59
   * @n When reading eHR type, range: 00-23
   * @n When reading eDOW type, range: 01-07
   * @n When reading eDATE type, range: 01-31
   * @n When reading eMTH type, range: 01-12
   * @n When reading eYR type, range: 2000-2099
   */
  uint16_t getTypeTime(eTimeType_t type);

  /**
   * @fn setTime
   * @brief According to the available array, set all the time
   * @param rtc Array in the following format (type is uint16_t)
   * @n rtc[0] for eSEC type, range: 00-59
   * @n rtc[1] for eMIN type, range: 00-59
   * @n rtc[2] for eHR type, range: 00-23
   * @n rtc[3] for eDOW type, range: 01-07
   * @n rtc[4] for eDATE type, range: 01-31
   * @n rtc[5] for eMTH type, range: 01-12
   * @n rtc[6] for eYR type, range: 2000-2099
   * @n Note: Values out of range will result in a setting error
   * @return None
   */
  void setTime(uint16_t *rtc);

  /**
   * @fn setTypeTime
   * @brief According to the set time type, upload the value of corresponding range, set time
   * @param type Time type to be obtained, eTimeType_t:
   * @n eSEC, eMIN, eHR, eDOW, eDATE, eMTH, eYR
   * @param typeTime According to the selected time type, write the value of the corresponding range
   * @n When setting eSEC type, range: 00-59
   * @n When setting eMIN type, range: 00-59
   * @n When setting eHR type, range: 00-23
   * @n When setting eDOW type, range: 01-07
   * @n When setting eDATE type, range: 01-31
   * @n When setting eMTH type, range: 01-12
   * @n When setting eYR type, range: 2000-2099
   * @n Note: Values out of range will result in a setting error
   * @return None
   */
  void setTypeTime(eTimeType_t type, uint16_t typeTime);

  /**
   * @fn start
   * @brief Start RTC timing function
   * @n this bit is part of the seconds byte
   * @return None
   */
  void start(void);

  /**
   * @fn stop
   * @brief Stop RTC timing function
   * @n this bit is part of the seconds byte
   * @return None
   */
  void stop(void);

  /**
   * @fn setSqwPinMode
   * @brief Control the operation of the SQW/OUT pin
   * @param mode SQW Pin output mode:
   * @n eSquareWave_LOW, eSquareWave_HIGH, eSquareWave_1Hz, 
   * @n eSquareWave_4kHz, eSquareWave_8kHz, eSquareWave_32kHz
   * @return None
   */
  void setSqwPinMode(eSqwPinMode_t mode);

  /**
   * @fn getSqwPinMode
   * @brief Get the current output mode of SQW/OUT pin
   * @return eSqwPinMode_t:
   * @n eSquareWave_LOW, eSquareWave_HIGH, eSquareWave_1Hz, 
   * @n eSquareWave_4kHz, eSquareWave_8kHz, eSquareWave_32kHz
   */
  eSqwPinMode_t getSqwPinMode(void);

  /**
   * @fn saveTimeToEEPROM
   * @brief Store the current time into EEPROM, which can be used to set the time to when last saved after powering off and restarting 
   * @n Call the interface when powering off and call setTimeFromEEPROM() to realize when restarting
   * @return None
   */
  void saveTimeToEEPROM(void);

  /**
   * @fn setTimeFromEEPROM
   * @brief Set the time to when last saved
   * @n If saveTimeToEEPROM() wasn’t called before or the contents in EEPROM have been modified after the call,
   * @n the time setting error may occur.
   * @note If the MCU board is reset when sensor is not powered off, the interface may be miscalled and the time may be changed by mistake.
   * @return None
   */
  void setTimeFromEEPROM(void);

  /**
   * @fn readEEPROM
   * @brief Read EEPROM through I2C bus
   * @param addr  EEPROM data address 8bits
   * @param pBuf Storage and buffer for data to be read
   * @param size Length of the data to be read
   * @return Return the read length, returning 0 indicates reading failed
   */
  size_t readEEPROM(uint8_t addr, uint8_t* pBuf, size_t size);

  /**
   * @fn writeEEPROM
   * @brief Write EEPROM through I2C bus
   * @param addr  EEPROM data address 8bits
   * @param pBuf Storage and buffer for data to be written
   * @param size Length of data to be written
   * @note The data size that users can freely store is 247 bytes, range 0-247
   * @n The last 8 bytes are for storing the time data saved when the user call saveTimeToEEPROM()
   * @return None
   */
  void writeEEPROM(uint8_t addr, const uint8_t* pBuf, size_t size);

protected:
  /**
   * @fn bcd2bin(uint8_t val)
   * @brief BCD code to BIN code
   * @param val Input BCD code
   * @return Return BIN code
   */
  static uint8_t bcd2bin(uint8_t val);

  /**
   * @fn bin2bcd(uint8_t val)
   * @brief BIN code to BCD code
   * @param val Input BIN code
   * @return Return BCD code
   */
  static uint8_t bin2bcd(uint8_t val);

  /**
   * @fn writeReg
   * @brief Write register value through I2C bus
   * @param reg  Register address 8bits
   * @param pBuf Storage and buffer for data to be written
   * @param size Length of data to be written
   * @param deviceAddr Select the I2C device address for communication, _ds1307Addr or _eepromAddr
   * @return None
   */
  void writeReg(uint8_t reg, const void* pBuf, size_t size, uint8_t deviceAddr=DS1307_I2C_ADDR);

  /**
   * @fn readReg
   * @brief Read register value through I2C bus
   * @param reg  Register address 8bits
   * @param pBuf Storage and buffer for data to be read
   * @param size Length of the data to be read
   * @param deviceAddr Select the I2C device address for communication, _ds1307Addr or _eepromAddr
   * @return Return the read length, returning 0 indicates reading failed
   */
  size_t readReg(uint8_t reg, void* pBuf, size_t size, uint8_t deviceAddr=DS1307_I2C_ADDR);

private:
  uint8_t rtcBCD[7], rtcMask[7];     // used prior to read/set ds1307 registers;
  TwoWire *_pWire;       // The pointer to I2C communication method
  uint8_t _ds1307Addr;   // I2C communication address of DS1307
  uint8_t _eepromAddr;   // I2C communication address of EEPROM
};
#endif
