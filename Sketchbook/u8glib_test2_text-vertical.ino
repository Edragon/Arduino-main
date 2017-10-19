// Henry's Bench Hello World - for use with Monochrome OLEDs

#include "U8glib.h"

//**************************************************
// Change this constructor to match your display!!!
// The arguments are the SPI pin # (SCK, MOSI, CS, C/D, RES)
U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9, 8);
//**************************************************

int yPos = 0;  //global variable

void setup() {  
  u8g.setFont(u8g_font_unifont);
  u8g.setColorIndex(1); // Instructs the display to draw with a pixel on. 
}

void loop() {  
  u8g.firstPage();
  do {  
    draw();
  } while( u8g.nextPage() );

  if(yPos < 83) {
    yPos++;  }
    else
    yPos = 0;
  }

void draw(){
  u8g.drawStr( 5, yPos, "Hello Irene");
 
}
