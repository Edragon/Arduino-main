/*
MD_MAX72xx - Library for using a MAX7219/7221 LED matrix controller
  
See header file for comments

This file contains class and hardware related methods.
  
Copyright (C) 2012-14 Marco Colli. All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <Arduino.h>
#include <SPI.h>
#include "MD_MAX72xx.h"
#include "MD_MAX72xx_lib.h"

/**
 * \file
 * \brief Implements class definition and general methods
 */

MD_MAX72XX::MD_MAX72XX(uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDevices):
_dataPin(dataPin), _clkPin(clkPin), _csPin(csPin), _hardwareSPI(false),
_maxDevices(numDevices), _updateEnabled(true)
#if USE_LOCAL_FONT && USE_FONT_INDEX
,_fontIndex(NULL)
#endif
{
}

MD_MAX72XX::MD_MAX72XX(uint8_t csPin, uint8_t numDevices):
_dataPin(0), _clkPin(0), _csPin(csPin), _hardwareSPI(true),
_maxDevices(numDevices), _updateEnabled(true)
#if USE_LOCAL_FONT && USE_FONT_INDEX
,_fontIndex(NULL)
#endif
{
}

void MD_MAX72XX::begin(void)
{
  // initialize the hardware
  if (_hardwareSPI)
  {
	SPI.begin();
	SPI.setDataMode(SPI_MODE0);
	SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
  }
  else
  {
    pinMode(_dataPin, OUTPUT);
    pinMode(_clkPin, OUTPUT);
  }

  // initialise our preferred CS pin (could be same as SS)
  digitalWrite(_csPin, HIGH);
  pinMode(_csPin, OUTPUT);

  // object memory and internals
  setShiftDataInCallback(NULL);
  setShiftDataOutCallback(NULL);

  _matrix = (deviceInfo_t *)malloc(sizeof(deviceInfo_t) * _maxDevices);
  _spiData = (uint8_t *)malloc(SPI_DATA_SIZE);

#if USE_LOCAL_FONT
#if USE_INDEX_FONT
  _fontIndex = (uint16_t *)malloc(sizeof(uint16_t) * FONT_INDEX_SIZE);
#endif
  setFont(NULL);
#endif // INCLUDE_LOCAL_FONT

  // Initialize the display devices. On initial power-up
  // - all control registers are reset, 
  // - scan limit is set to one digit (row/col or LED),
  // - Decoding mode is off, 
  // - intensity is set to the minimum, 
  // - the display is blanked, and 
  // - the MAX7219/MAX7221 is shut down.
  // The devices need to be set to our library defaults prior using the 
  // display modules.
  control(TEST, OFF);				// no test
  control(SCANLIMIT, ROW_SIZE-1);	// scan limit is set to max on startup
  control(INTENSITY, MAX_INTENSITY/2);	// set intensity to a reasonable value
  control(DECODE, OFF);				// make sure that no decoding happens (warm boot potential issue)
  clear();					// Clear the display and internal memory map
  control(SHUTDOWN, OFF);			// take the modules out of shutdown mode
}

MD_MAX72XX::~MD_MAX72XX(void)
{
	if (_hardwareSPI) SPI.end();	// reset SPI mode

	free(_matrix);
	free(_spiData);
#if USE_LOCAL_FONT && USE_FONT_INDEX
	if (_fontIndex != NULL) free(_fontIndex);
#endif
}

void MD_MAX72XX::controlHardware(uint8_t dev, controlRequest_t mode, int value)
// control command is for the devices, translate internal request to device bytes 
// into the transmission buffer
{
	uint8_t opcode = OP_NOOP;
	uint8_t param = 0;

	// work out data to write
	switch (mode)
	{
		case SHUTDOWN:
			opcode = OP_SHUTDOWN;	
			param = (value == OFF ? 1 : 0);
			break;
	
		case SCANLIMIT:
			opcode = OP_SCANLIMIT;
			param = (value > MAX_SCANLIMIT ? MAX_SCANLIMIT : value);
			break;
	
		case INTENSITY:
			opcode = OP_INTENSITY;
			param = (value > MAX_INTENSITY ? MAX_INTENSITY : value);
			break;
	
		case DECODE:
			opcode = OP_DECODEMODE;
			param = (value == OFF ? 0 : 0xff);
			break;
	
		case TEST:
			opcode = OP_DISPLAYTEST;
			param = (value == OFF ? 0 : 1);
			break;

		default:
			return;
	}

  // put our device data into the buffer
  _spiData[SPI_OFFSET(dev, 0)] = opcode;
  _spiData[SPI_OFFSET(dev, 1)] = param;
}

void MD_MAX72XX::controlLibrary(controlRequest_t mode, int value)
// control command was internal, set required parameters
{
  switch (mode)
  {
	  case UPDATE:
		  _updateEnabled = (value == ON);
		  if (_updateEnabled) flushBufferAll();
		  break;

	  case WRAPAROUND:
		  _wrapAround = (value == ON);
		  break;

	  default:
		  break;
  }
}

bool MD_MAX72XX::control(uint8_t startDev, uint8_t endDev, controlRequest_t mode, int value) 
{
  if (endDev < startDev) return(false);

  if (mode < UPDATE)	// device based control
  {
	spiClearBuffer();
    for (uint8_t i = startDev; i <= endDev; i++) 
	  controlHardware(i, mode, value); 
	spiSend();
  }
  else					// internal control function, doesn't relate to specific device
  {
    controlLibrary(mode, value);
  }
  
  return(true);
}
  
bool MD_MAX72XX::control(uint8_t buf, controlRequest_t mode, int value)
// dev is zero based and needs adjustment if used
{
  if (buf > LAST_BUFFER) return(false);
  
  if (mode < UPDATE)	// device based control
  {
	  spiClearBuffer();
	  controlHardware(buf, mode, value);
	  spiSend();
  }
  else					// internal control function, doesn't relate to specific device
  {
	  controlLibrary(mode, value);
  }
  
  return(true);
}

void MD_MAX72XX::flushBufferAll()
// Only one data byte is sent to a device, so if there are many changes, it is more
// efficient to send a data byte all devices at the same time, substantially cutting 
// the number of communication messages required.
{
  for (uint8_t i=0; i<ROW_SIZE; i++)	// all data rows
  {
    bool bChange = false;	// set to true if we detected a change

	spiClearBuffer();

    for (uint8_t dev = FIRST_BUFFER; dev <= LAST_BUFFER; dev++)	// all devices
    {
      if (bitRead(_matrix[dev].changed, i))
	  {
	    // put our device data into the buffer
		_spiData[SPI_OFFSET(dev, 0)] = OP_DIGIT0+i;
		_spiData[SPI_OFFSET(dev, 1)] = _matrix[dev].dig[i];
		bChange = true;
	  }
    }

	if (bChange) spiSend();
  }

  // mark everything as cleared
  for (uint8_t dev = FIRST_BUFFER; dev <= LAST_BUFFER; dev++)
	_matrix[dev].changed = ALL_CLEAR;
}

void MD_MAX72XX::flushBuffer(uint8_t buf)
// Use this function when the changes are limited to one device only.
// Address passed is a buffer address
{
  PRINT("\nflushBuf: ", buf);
  PRINTS(" r");

  if (buf > LAST_BUFFER) 
    return;

  for (uint8_t i = 0; i < ROW_SIZE; i++)
  {
    if (bitRead(_matrix[buf].changed, i))
	{
	  PRINT("", i);
      spiClearBuffer();

      // put our device data into the buffer
      _spiData[SPI_OFFSET(buf, 0)] = OP_DIGIT0+i;
      _spiData[SPI_OFFSET(buf, 1)] = _matrix[buf].dig[i];
    
      spiSend();
    }
  }
  _matrix[buf].changed = ALL_CLEAR;
}

void MD_MAX72XX::spiClearBuffer(void)
// Clear out the spi data array
{
	memset(_spiData, OP_NOOP, SPI_DATA_SIZE);
}

void MD_MAX72XX::spiSend() 
{
  // enable the devices to receive data
  digitalWrite(_csPin, LOW);

  // shift out the data 
  if (_hardwareSPI)
  {
#ifdef ARDUINO_ARCH_ESP8266
    SPI.writeBytes(_spiData, SPI_DATA_SIZE);
#else
    SPI.transfer(_spiData, SPI_DATA_SIZE);
#endif
  }
  else
  {
    for (int i = 0; i < SPI_DATA_SIZE; i++)
      shiftOut(_dataPin, _clkPin, MSBFIRST, _spiData[i]);
  }
		
  // latch the data onto the display
  digitalWrite(_csPin, HIGH);
}    
