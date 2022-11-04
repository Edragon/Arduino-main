/*
 * This demo code only demostrate it is working by read analog value, not able to get correct current value
 * read a hair dry will get 2.4V~2.55V varying voltage, the amlification should be used for calculation.
 */

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(5, 3, 6);

float voltage_0;
float voltage_1;
float voltage_2;
float btn;
int pressed_btn;

void setup()   {
  Serial.begin(9600);

  display.begin();
  display.setContrast(50);

}


void loop() {
  read_analog();
  read_button();
  update_display();
  
}


