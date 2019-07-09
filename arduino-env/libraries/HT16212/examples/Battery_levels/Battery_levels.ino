/*
  Battery Levels

  Displays the various battery levels with 500ms
  pause between.

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
  lcd.setBatteryLevel(1);
  delay(500);
  lcd.setBatteryLevel(2);
  delay(500);
  lcd.setBatteryLevel(3);
  delay(500);
}
