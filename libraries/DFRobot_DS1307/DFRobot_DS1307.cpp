/*!
 * @file  DFRobot_DS1307.cpp
 * @brief  Define the basic structure of class DFRobot_DS1307
 * @details  The clock/calendar provides seconds, minutes, hours, 
 * @n  day, date, month, and year information.
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2021-11-18
 * @url  https://github.com/DFRobot/DFRobot_DS1307
 */
#include "DFRobot_DS1307.h"

DFRobot_DS1307::DFRobot_DS1307(TwoWire *pWire, uint8_t ds1307Addr)
{
  _ds1307Addr = ds1307Addr;
  _eepromAddr = EEPROM_I2C_ADDR;
  _pWire = pWire;
  rtcMask[eSEC] = DS1307_SEC_MASK;
  rtcMask[eMIN] = DS1307_MIN_MASK;
  rtcMask[eHR] = DS1307_HR_MASK;
  rtcMask[eDOW] = DS1307_DOW_MASK;
  rtcMask[eDATE] = DS1307_DATE_MASK;
  rtcMask[eMTH] = DS1307_MTH_MASK;
  rtcMask[eYR] = DS1307_YR_MASK;
}

bool DFRobot_DS1307::begin(void)
{
  _pWire->begin();   // Wire.h(I2C)library function initialize wire library
  delay(100);

  if(0 == readReg(DS1307_SEC_REG, &rtcBCD[eSEC], sizeof(rtcBCD[eSEC]))){   // Judge whether the data bus is successful
    DBG("ERR_DATA_BUS");
    return false;
  }
  start();

  DBG("begin ok!");
  return true;
}

void DFRobot_DS1307::getTime(uint16_t *rtc)
{
  readReg(DS1307_SEC_REG, rtcBCD, sizeof(rtcBCD));
  for(uint8_t i=0; i<7; i++){  // cycle through each component, create array of data
    rtc[i] = (uint16_t)(bcd2bin(rtcBCD[i] & rtcMask[i]));
  }
  rtc[eYR] += DS1307_BASE_YR;
}

uint16_t DFRobot_DS1307::getTypeTime(eTimeType_t type)
{
  uint8_t time = 0;
  uint16_t typeTime = 0;

  readReg(type, &time, sizeof(time));
  typeTime = (uint16_t)(bcd2bin(time & rtcMask[type]));

  if(eYR == type){
    typeTime += DS1307_BASE_YR;
  }

  return typeTime;
}

void DFRobot_DS1307::setTime(uint16_t *rtc)
{
  rtc[eYR] -= DS1307_BASE_YR;
  for(uint8_t i=0; i<7; i++){  // cycle through each component, create array of data
    rtcBCD[i] = (rtcBCD[i] & (~rtcMask[i])) | (bin2bcd((uint8_t)rtc[i] & rtcMask[i]));
  }
  writeReg(DS1307_SEC_REG, rtcBCD, sizeof(rtcBCD));
}

void DFRobot_DS1307::setTypeTime(eTimeType_t type, uint16_t typeTime)
{
  if(eYR == type){
    typeTime -= DS1307_BASE_YR;
  }

  rtcBCD[type] = (rtcBCD[type] & (~rtcMask[type])) | (bin2bcd((uint8_t)typeTime & rtcMask[type]));

  writeReg(type, &rtcBCD[type], sizeof(rtcBCD[type]));
}

void DFRobot_DS1307::start(void)
{
  // TODO : preserve existing seconds
  rtcBCD[eSEC] &= ~DS1307_CLOCKHALT;
  writeReg(DS1307_SEC_REG, &rtcBCD[eSEC], sizeof(rtcBCD[eSEC]));
}

void DFRobot_DS1307::stop(void)
{
  rtcBCD[eSEC] |= DS1307_CLOCKHALT;
  writeReg(DS1307_SEC_REG, &rtcBCD[eSEC], sizeof(rtcBCD[eSEC]));
}

void DFRobot_DS1307::setSqwPinMode(eSqwPinMode_t mode)
{
  writeReg(DS1307_CTL_REG, &mode, sizeof(mode));
}

DFRobot_DS1307::eSqwPinMode_t DFRobot_DS1307::getSqwPinMode(void)
{
  eSqwPinMode_t mode;
  readReg(DS1307_CTL_REG, &mode, sizeof(mode));
  return mode;
}

void DFRobot_DS1307::saveTimeToEEPROM(void)
{
  readReg(DS1307_SEC_REG, rtcBCD, sizeof(rtcBCD));
  writeEEPROM(248, rtcBCD, sizeof(rtcBCD));   // Use the last 8 bites of EEPROM to store time
  // delay(1);
}

void DFRobot_DS1307::setTimeFromEEPROM(void)
{
  readEEPROM(248, rtcBCD, sizeof(rtcBCD));
  writeReg(DS1307_SEC_REG, rtcBCD, sizeof(rtcBCD));   // The last 8 bytes of the EEPROM are the time stored before
  // delay(1);
}

void DFRobot_DS1307::writeEEPROM(uint8_t addr, const uint8_t* pBuf, size_t size)
{
  while(size > 8){
    writeReg(addr, pBuf, 8, EEPROM_I2C_ADDR);
    addr += 8;
    pBuf += 8;
    size -= 8;
    delay(1);
  }
  writeReg(addr, pBuf, size, EEPROM_I2C_ADDR);
  delay(1);
}

size_t DFRobot_DS1307::readEEPROM(uint8_t addr, uint8_t* pBuf, size_t size)
{
  return readReg(addr, pBuf, size, EEPROM_I2C_ADDR);
  // delay(1);
}

uint8_t DFRobot_DS1307::bcd2bin(uint8_t val)
{
    return val - 6 * (val >> 4);
}

uint8_t DFRobot_DS1307::bin2bcd(uint8_t val)
{
    return val + 6 * (val / 10);
}

void DFRobot_DS1307::writeReg(uint8_t reg, const void* pBuf, size_t size, uint8_t deviceAddr)
{
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;

  _pWire->beginTransmission(deviceAddr);
  // _pWire -> write(0);
  _pWire->write(reg);

  // DBG(size);
  for(size_t i = 0; i < size; i++){
    // DBG(_pBuf[i]);
    _pWire->write(_pBuf[i]);
  }
  _pWire->endTransmission();
}

size_t DFRobot_DS1307::readReg(uint8_t reg, void* pBuf, size_t size, uint8_t deviceAddr)
{
  size_t count = 0;
  if(NULL == pBuf){
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t*)pBuf;

  // Serial.print("reg: ");
  // DBG(reg);
  _pWire->beginTransmission(deviceAddr);
  // _pWire -> write(0);
  _pWire -> write(reg);
  if(0 != _pWire->endTransmission()){   // Used Wire.endTransmission() to end a slave transmission started by beginTransmission() and arranged by write().
    DBG("endTransmission ERROR!!");
  }else{
    _pWire->requestFrom(deviceAddr, (uint8_t)size);   // Master device requests size bytes from slave device, which can be accepted by master device with read() or available()
    
    while (_pWire->available()){
      _pBuf[count++] = _pWire->read();   // Use read() to receive and put into buf
    }
  }
  return count;
}
