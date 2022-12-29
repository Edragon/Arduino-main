
#define relay 1
#define led 0

#define current_sensor 9
#define voltage_sensor 10
#define buttons 8


#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

SSD1306AsciiWire oled;

//int flag = 1;

void setup() {
  Wire.begin();
  Wire.setClock(400000L);

  oled.begin(&Adafruit128x64, 0x3C);

  oled.setFont(Adafruit5x7);

  //pinMode(relay, OUTPUT);
  //pinMode(led, OUTPUT);
}

void loop() {

  int cs = analogRead(current_sensor);
  int vs = analogRead(voltage_sensor);
  int bt = analogRead(buttons);

  oled.clear();
  oled.set2X();
  oled.println("SVC1039!");
  oled.println("--------------");

  oled.set1X();

  oled.print("current: ");
  oled.println(cs - 512);

  oled.print("voltage: ");
  oled.println(vs);

  oled.print("buttons: ");
  oled.println(bt);

//  if (flag = 1) {
//    flag = 0;
//    digitalWrite(relay, HIGH);
//    digitalWrite(led, HIGH);
//  } else {
//    flag = 1;
//    digitalWrite(relay, LOW);
//    digitalWrite(led, LOW);
//  }
  delay(1000);

}
