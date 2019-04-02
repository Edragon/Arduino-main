#include "Ht1621.h"

const byte CS_PIN =   2;  //chip selection output
const byte WR_PIN =   3;  //read clock output
const byte DATA_PIN = 4;  //Serial data output
const byte BKLT_PIN = 5;  //Backlight

const byte BKLT_TIMEOUT = 5;  //Backlight timeout


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
  if ( n > 1999 ){ n = -1; }
  ht.writeNumber(++n);
  delay(200);

  if ( n % 100 == 0 ){ 
      waitTill = millis() + BKLT_TIMEOUT * 1000 + 2000;
  }
}



// Emacs control
// Local Variables:
// eval: (c++-mode)
// End:
