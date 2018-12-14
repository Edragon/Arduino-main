/*

See usage instructions in ht1621.h

*/
#include "Arduino.h"
#include "Ht1621.h"
//#ifndef byte
//#define byte unsigned char 
//#endif

//===========================================================================
// private
inline void Ht1621::writeBits(byte sdata, byte cnt){ 
  Ht1621::writeBits(sdata, cnt, 1);
}
inline void Ht1621::writeBits(byte sdata, byte cnt, bool bklt){ 
  byte i; 
  for(i=0; i<cnt; i++){ 
    digitalWrite(wrPin, LOW);
    digitalWrite(dataPin, sdata & 0x80);
    digitalWrite(wrPin, HIGH);
    sdata<<=1; 
  } 

  if ( bklt ){
    digitalWrite(bkltPin, HIGH);
    bkltOffTime = millis() + bkltTimeout * 1000;
  }
}

  
inline void Ht1621::writeCmd(byte command){ 
  digitalWrite(csPin, LOW);
  writeBits(0x80,    4, 0);   
  writeBits(command, 8, 0);
  digitalWrite(csPin, HIGH);
}
 
//===========================================================================
// public

//---------------------------------------------------------------------------
// Constructor
//
/* If you are short on RAM you can make the varibales for the pins and
backlight timout #defs.  Remove them from the constructor and from
when it is called and add #defs for csPin etc. This will save 5 bytes
of RAM.  E.G.:

Ht1621::Ht1621(){
    bkltOffTime  = 0;
}

#define csPin        2
#define wrPin        3
#define dataPin      4
#define bkltPin      5
#define bkltTimeout 20
*/

Ht1621::Ht1621(const byte csPinIn, const byte wrPinIn, const byte dataPinIn,
	      const byte bkltPinIn, const byte bkltTimeoutIn){

    csPin       = csPinIn;
    wrPin       = wrPinIn;
    dataPin     = dataPinIn;
    bkltPin     = bkltPinIn;
    bkltTimeout = bkltTimeoutIn;
    bkltOffTime  = 0;
  }


//---------------------------------------------------------------------------
//Setup needs to be called before use.  This can not be in the
//constructor because the audino isn't ready for pinMode etc.
void Ht1621::init(void){
  pinMode(csPin,   OUTPUT); 
  pinMode(wrPin,   OUTPUT);  
  pinMode(dataPin, OUTPUT); 
  pinMode(bkltPin, OUTPUT); 
  
  digitalWrite(bkltPin, HIGH);
  digitalWrite(csPin,   HIGH);
  digitalWrite(dataPin, HIGH) ;
  digitalWrite(wrPin,   HIGH);
  delay(50);
  writeCmd(Sys_en);
  writeCmd(RCosc);    
  writeCmd(ComMode);  
  writeCmd(LCD_on);
  setAll(0x00);
}   
  

//---------------------------------------------------------------------------
bool Ht1621::poll(void){ Ht1621::poll( millis() ); }
bool Ht1621::poll(unsigned long timeNow){
  if ( timeNow > bkltOffTime ){
    digitalWrite(bkltPin, LOW);
    return 0;
  }
  return 1;
}


//---------------------------------------------------------------------------
void Ht1621::shutDown(void){ 
    setAll(0);
    digitalWrite(bkltPin, LOW);
}



//---------------------------------------------------------------------------
//Write a byte of data to the address given.  The HT1621 has 32
//4-bit words, one word for each segment it can control. THe four
//bits are four seperate "common anodes" (there is also a pin for
//each of these on the controller).  In general, two words are used
//per digit, 7 bits for the 7 segments and one for a dot or the
//"half-digit".  The digits on this board use all four common
//annodes and so only use 2 4-bit words but each word starts on a
//2-byte boundry (addresses 0, 4, and 8). 
void Ht1621::writeData(byte addr, byte sdata){ 
  Ht1621::writeData(addr, sdata, 1);
}

void Ht1621::writeData(byte addr, byte sdata, bool bklt){ 
  digitalWrite(csPin, LOW);
  writeBits(0b10100000, 3, bklt);    
  writeBits(addr<<3,    6, bklt);     
  writeBits(sdata,      8, bklt);    
  digitalWrite(csPin, HIGH);
} 
  
//---------------------------------------------------------------------------
//Set every digit in the display
void Ht1621::setAll(byte d){ Ht1621::setAll(d, 1); }
void Ht1621::setAll(byte d, bool bklt){
  byte i; 
  for(i = 0 ; i < NUM_DIGITS; i++){ 
    writeData(i, d, bklt); 
  } 
}
  
//---------------------------------------------------------------------------
// Display a positive integer in the range 0 - 1999. Assumes
// NUM_DIGITS == 3.  Also works arround the extra space for the dot
// between the right hand two digits if possible. 
void Ht1621::writeNumber(int n){ Ht1621::writeNumber(n, 1); }
void Ht1621::writeNumber(int n, bool bklt){
  byte d[3] = {0, 0, 0};
    
  //If less than 200, don't use right hand digit because of the dot
  // shifting it to the right a bit
  if ( n < 10 ){
    d[1] = digit[n];
  } else if ( n <= 99 ){
    d[0] = digit[n / 10];
    d[1] = digit[n % 10];
  } else if ( n <= 199 ){
    n -= 100;
    d[0] = digit[n / 10] | 0x80;
    d[1] = digit[n % 10];
  } else {  //n > 199
    if ( n >= 1000 ){ d[0] = 0x80; }
    d[2]  = digit[n % 10]; n /= 10;
    d[1]  = digit[n % 10]; n /= 10;
    d[0] |= digit[n % 10];
  }
    
  writeData(0, d[0], bklt);
  writeData(1, d[1], bklt);
  writeData(2, d[2], bklt);
}


//                           0    1    2    3    4    5    6    7    8    9
const byte Ht1621::digit[]={0x5f,0x06,0x6b,0x2f,0x36,0x3d,0x7d,0x07,0x7f,0x3f};

//===========================================================================
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#if STAND_ALONE
static const byte CS_PIN =   2;  //chip selection output
static const byte WR_PIN =   3;  //read clock output
static const byte DATA_PIN = 4;  //Serial data output
static const byte BKLT_PIN = 5;  //Backlight

static const byte BKLT_TIMEOUT = 5;  //Backlight timeout


//Declare an Ht1621 object. If you have more than one display, you can
//declare several (untested).
Ht1621 ht(CS_PIN, WR_PIN, DATA_PIN, BKLT_PIN, BKLT_TIMEOUT);


//---------------------------------------------------------------------------
void setup(){
  ht.init();
  ht.setAll(0x00);
  ht.writeData(0, 0x5E);
  ht.writeData(1, 0x6C);
  ht.writeData(2, 0x58);
  delay(2000);

  ht.setAll(0x00);
  ht.writeData(0, 0x3D);
  ht.writeData(1, 0x64);
  ht.writeData(2, 0x6E);
  delay(2000);

  ht.setAll(0x00);
  ht.writeData(0, 0x71);
  ht.writeData(1, 0x77);
  ht.writeData(2, 0x6E);
  delay(2000);

}


//===========================================================================
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void loop() {
  static int n = 0;
  static unsigned long waitTill = 0;

  ht.poll();
  if ( waitTill && millis() < waitTill ){ return; }
  if ( n >= 1999 ){ n = -1; }
  ht.writeNumber(++n);
  delay(200);

  if ( n % 100 == 0 ){ 
      waitTill = millis() + BKLT_TIMEOUT * 1000 + 2000;
  }
}
#endif

// Emacs control
// Local Variables:
// eval: (c++-mode)
// End:


