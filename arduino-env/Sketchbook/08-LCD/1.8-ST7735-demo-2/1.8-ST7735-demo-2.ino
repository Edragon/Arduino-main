/*
  Arduino TFT text example

  This example demonstrates how to draw text on the
  TFT with an Arduino. The Arduino reads the value
  of an analog sensor attached to pin A0, and writes
  the value to the LCD screen, updating every
  quarter second.

  This example code is in the public domain

  Created 15 April 2013 by Scott Fitzgerald

  http://www.arduino.cc/en/Tutorial/TFTDisplayText

*/

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the Uno
#define cs   8
#define dc   10
#define rst  9

// pin definition for the Leonardo
// #define cs   7
// #define dc   0
// #define rst  1

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

// char array to print to the screen
char sensorPrintout[4];

void setup() {

  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);
}

void write_line_1 (int title, int data, int title_2, int data_2, int title_3, int data_3, int title_4, int data_4) {
  TFTscreen.setTextSize(1);
  TFTscreen.stroke(255, 255, 255);

  TFTscreen.text(title, 0, 0);
  TFTscreen.text(title_2, 80, 0);

  TFTscreen.text(title_3, 0, 15);
  TFTscreen.text(title_4, 80, 15);

  // data reflash
  TFTscreen.text(data, 30, 0);
  TFTscreen.text(data_2, 110, 0);
  
  TFTscreen.text(data_3, 30, 15);
  TFTscreen.text(data_4, 110, 15);
  delay(100);

  TFTscreen.stroke(0, 0, 0);

  TFTscreen.text(data, 30, 0);
  TFTscreen.text(data_2, 110, 0);
  
  TFTscreen.text(data_3, 30, 15);
  TFTscreen.text(data_4, 110, 15);
}

void loop() {
  write_line_1("P:", "250", "V:", "255", "C:", "252", "PREG:", "250");
  write_line_1("P:", "253", "V:", "251", "C:", "250", "PREG:", "150");
}
