
#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

#define cs   9
#define dc   3
#define rst  10

TFT TFTscreen = TFT(cs, dc, rst);




void setup() {

  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();

  TFTscreen.background(0, 0, 0);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.setTextSize(2);
  TFTscreen.text("Channel 1: \n ", 0, 0);
  TFTscreen.text("Channel 2: \n ", 0, 40);
  TFTscreen.text("Channel 3: \n ", 0, 80);

  Serial.begin(9600);

}

void loop() {


  reflash_read();

}

void reflash_read() {
  char volt_char_0[] = "    ";
  char volt_char_1[] = "    ";
  char volt_char_2[] = "    ";

  int sen_0 = analogRead(A0);
  int sen_1 = analogRead(A1);
  int sen_2 = analogRead(A2);

  float voltage_0 = sen_0 * (5.0 / 1023.0);
  float voltage_1 = sen_1 * (5.0 / 1023.0);
  float voltage_2 = sen_2 * (5.0 / 1023.0);

  dtostrf(voltage_0, 4, 2, volt_char_0);         // data to string
  dtostrf(voltage_1, 4, 2, volt_char_1);         // data to string
  dtostrf(voltage_2, 4, 2, volt_char_2);         // data to string
 
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(volt_char_0, 0, 20);
  TFTscreen.text(volt_char_1, 0, 60);
  TFTscreen.text(volt_char_2, 0, 100);
  delay(250);

  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(volt_char_0, 0, 20);
  TFTscreen.text(volt_char_1, 0, 60);
  TFTscreen.text(volt_char_2, 0, 100);

}

