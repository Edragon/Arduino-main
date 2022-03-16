/*
TM1650.cpp - Library implementation for TM1650.

Part of the TM16xx library by Maxint. See https://github.com/maxint-rd/TM16xx
The Arduino TM16xx library supports LED & KEY and LED Matrix modules based on TM1638, TM1637, TM1640 as well as individual chips.
Simply use print() on 7-segment displays and use Adafruit GFX on matrix displays.

Adjusted for TM1650 by Maxint R&D, based on TM1637 code. 
Partially based on TM1640 library by MRicardo Batista. See https://github.com/rjbatista/tm1638-library
*/

#ifndef TM1650_h
#define TM1650_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "TM16xx.h"

#define TM1650_MAX_POS 4

// TM1650 has two display modes: 8 seg x 4 grd and 7 seg x 4 grd
#define TM1650_DISPMODE_4x8 0x01
#define TM1650_DISPMODE_4x7 0x09

#define TM1650_CMD_MODE  0x48
#define TM1650_CMD_DATA_READ  0x49
#define TM1650_CMD_ADDRESS  0x68


class TM1650 : public TM16xx
{
  public:
    /** Instantiate a TM1650 module specifying the  data and clock pins, number of digits, display state, the starting intensity (0-7). */
    TM1650(byte dataPin, byte clockPin, byte numDigits=4, boolean activateDisplay=true, byte intensity=7, byte displaymode = TM1650_DISPMODE_4x8);
    virtual void clearDisplay();
    virtual void setupDisplay(boolean active, byte intensity);
    virtual uint32_t getButtons();

  protected:
#if defined(__AVR_ATtiny85__) ||  defined(__AVR_ATtiny13__) ||  defined(__AVR_ATtiny44__)
		// On slow processors we may not need this bitDelay, so save some flash
#else
    virtual void bitDelay();
#endif
    virtual void start();
    virtual void stop();
    virtual void send(byte data);
    virtual void sendData(byte address, byte data);
    virtual byte receive();
};

#endif
