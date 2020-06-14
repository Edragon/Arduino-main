#include <SPI.h>
#include "e-ink-2.13.h"
#include "imagedata.h"
#include "e-ink-display.h"

#define COLORED     0
#define UNCOLORED   1
  Epd epd;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);


  if (epd.Init() != 0) {
    Serial.print("e-Paper init failed");
    return;
  }
  epd.ClearFrame();
epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);

/*
  paint.Clear(COLORED);
  paint.DrawStringAt(8, 2, "Hello world", &Font12, UNCOLORED);
  epd.SetPartialWindowRed(paint.GetImage(), 0, 24, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
*/
}

void loop() {
  // put your main code here, to run repeatedly:
}

