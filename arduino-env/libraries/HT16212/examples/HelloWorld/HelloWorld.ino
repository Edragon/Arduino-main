/*
  Hello World!
  Displays the milliseconds passed since power on.
  Using an HT1621 based 7-segment LCD with 6 digits
  The circuit:
  cs to pin 13
  wr to pin 12
  Data to pin 8
  backlight to pin 10
  Created 9 dec 2018
  By valerio\new (5N44P)

  https://github.com/5N44P/ht1621-7-seg
*/

#include <HT1621.h> // include our library


HT1621 lcd; // create an "lcd" object


void setup(){
  // start the lcd:
  // cs to pin 13
  // wr to pin 12
  // Data to pin 8
  // backlight to pin 10
  // you can chose whichever pin you want



  lcd.begin(13, 12, 8, 10); // (cs, wr, Data, backlight)
  // if no backlight control is given, you can also use:
  // lcd.begin(13, 12, 8); // (cs, wr, Data)

  lcd.backlight(); // turn on the backlight led

  lcd.clear(); // clear the screen
}

void loop(){
  lcd.print(millis()/1000.0, 3); // print the floating point seconds value with 3 decimal digits precision
  delay(1); // wait 1 millisecond
}
