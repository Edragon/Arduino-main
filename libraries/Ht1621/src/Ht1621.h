#ifndef _HT1621_H
#define _HT1621_H
/*

The display (SKU 431141 from dx.com -US$2.52) this is setup for has 3
primary digits, from left to right addressed as 0, 1 and 2.  The main
bits are:
  
    0                a
    -                -
4 |   | 1        e |   | b
   -5-              -f-
6 |   | 2        g |   | c
    -                -
    3                d

In addition, digit 0, bit 7 turns on the "half digit" at the left hand
end of the display.  Digit 2 bit 7 turns on a decimal point between
the two right hand digits.  This somewhat odd arrangement of bits
comes from the way the display is wired to the controler and he
softwares assumption that the bits run in order accross words in the
memory of the controler where as, they actually run egfdcba
(lsb->msb).  Anyway, all that is dealt with by the above allocation of
segments.  If your display is different, use 

    7654 3210
0 = 0101 1111  0x5F
1 = 0000 0110  0x06
2 = 0110 1011  0x6B
3 = 0010 1111  0x2F
4 = 0011 0110  0x36
5 = 0011 1101  0x3D
6 = 0111 1101  0x7D
7 = 0000 0111  0x07
8 = 0111 1111  0x7F
9 = 0011 1111  0x3F

Example Pseudo Letters
V = 0101 1110  0x5E
o = 0110 1100  0x6C
l = 0101 1000  0x58
s = 0011 1101  0x3D
n = 0110 0100  0x64
d = 0110 1110  0x6E
f = 0111 0001  0x71
a = 0111 0111  0x77
d = 0110 1110  0x6E
t = 0111 1000  0x78
? = 0001 0111  0x17

***************************************************************************
*Usage: Either:
*       Place this file and ht1621.ino in your project.
*       Add #include "ht1621.h" to the top of your main project file
*
*    or install the ht1621lib.zip library into your arduino IDE 
*
*    or add ht1621.h and ht1621.cpp to your source direcotry and build with
*       make or whatever compilation processor you normally use
*
*    or set STAND_ALONE to 1 and this file becomes an example you can use 
*       as the basis for developing your own application
*    
*   In your sketch/C++ code:
*       Declare one or more Ht1621 objects using the pins and the
*       backlight timeout as described below.
*       as arguments.
*
*       e.g. Ht1621 ht(3, 4, 5, 6, 20). 


Notes: 

 The normal range for the backlight is 1 - 254 (seconds).  If you're
 not using the back light, or are managing it outside this library,
 set it to 0. To set it permanantly on, set it to 255.  If the range
 is not big enough, replace byte with unsigned short int in the
 declaration of bkltTimeout, bkltTimeoutIn, and BKLT_MAX below.

1. If you are tight on program space but have some RAM to spare,
remove the inline clauses.

2. If you are short on RAM, see the notes in ht1621.cpp on the
constructor.
*/
#include "Arduino.h"

//---------------------------------------------------------------------------
// Constants
typedef unsigned char byte;
#ifndef HIGH
#define HIGH (1)
#define LOW  (0)
#endif
#define STAND_ALONE 0             //set to one to run this library as a complete
                                  // program on its own.  Normally set to 0
static const byte NUM_DIGITS = 3; //Number of 7 segment digits


//---------------------------------------------------------------------------
class Ht1621{
//-----------------------------------
// private: 
  static const byte ComMode    = 0x52;
  static const byte RCosc      = 0x30; 
  static const byte LCD_on     = 0x06;
  static const byte LCD_off    = 0x04;
  static const byte Sys_en     = 0x02;
  static const byte CTRl_cmd   = 0x80;
  static const byte Data_cmd   = 0xa0;
  static const byte digit[];

  byte csPin, wrPin, dataPin, bkltPin;
  byte bkltTimeout;
  static const byte BKLT_MAX = ~0;
  unsigned long int bkltOffTime;

 
   void writeBits(byte sdata, byte cnt);  
  void writeBits(byte sdata, byte cnt, bool bkLt);  
  void writeCmd(byte command);
 
//-------------------------------------
public:
  //Constructor.
  Ht1621(const byte csPinIn, const byte wrPinIn, const byte dataPinIn,
	 const byte bkltPinIn, const byte bkltTimeoutIn);

  //init() needs to be called before use.  This can not be in the
  //constructor because the audino isn't ready for pinMode etc.
  void init(void);   
  

  //Call this frequently (e.g. from loop().  Currently, it jsut
  //manages the back light timeout so, if you;re not using that you
  //don't need to call it.  The more you care about the accuracy of
  //the back lighttimeout, the more frequently you should call this
  //routine.  If timeNow is provided, it is used insted of the current
  //value of millis(). Returns true if the back light is on.
  bool poll(void);
  bool poll(unsigned long timeNow);


  //---------------------------------------------------------------------------
  // Call shutdown() to put the display into low power mode.  Call
  // init() to wake it up again.
  void shutDown(void);


  //---------------------------------------------------------------------------
  //Write a byte of data to the address given.  The HT1621 has 32
  //4-bit words, one word for each segment it can control. The four
  //bits are four seperate "common anodes" (there is also a pin for
  //each of these on the controller).  In general, two words are used
  //per digit, 7 bits for the 7 segments and one for a dot or the
  //"half-digit".  The digits on this board use all four common
  //annodes and so only use 2 4-bit words but each word starts on a
  //2-byte boundry (addresses 0, 4, and 8).  If bklt is omitted, it
  //defaults to true, that is, turn on the back light for bkltTimeout
  //seconds.
  void writeData(byte addr, byte sdata);
  void writeData(byte addr, byte sdata, bool bklt);
  
  //---------------------------------------------------------------------------
  //Set every segement in every digit in the display. If bklt is
  //omitted, it defaults to true, that is, turn on the back light for
  //bkltTimeout seconds.
  void setAll(byte d);
  void setAll(byte d, bool bklt);
  
  //---------------------------------------------------------------------------
  //Display a positive integer in the range 0 - 1999. Assumes
  //NUM_DIGITS == 3.  Also works arround the extra space for the dot
  //between the right hand two digits if possible. If bklt is omitted,
  //it defailts to true, that is, turn on the back light for
  //bkltTimeout seconds.
  void writeNumber(int n);
  void writeNumber(int n, bool bklt);
};

#endif   

// Emacs control
// Local Variables:
// eval: (c++-mode)
// End:


