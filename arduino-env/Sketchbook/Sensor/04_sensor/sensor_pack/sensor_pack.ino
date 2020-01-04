#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bme; // I2C

//////////////////////////////////////////////////////////  BMP

#include <Wire.h> //IIC

#include <math.h>

int BH1750address = 0x23;

byte buff[2];

//////////////////////////////////////////////////////////  BH1750

#include <AM232X.h>

AM232X AM2322;

//////////////////////////////////////////////////////////  AM2322

void am2322_print();
void bmp085Calibration();
void bmp_print();
void bh1750_print();

void setup() {
  Serial.begin(9600);
  Wire.begin();
  if (!bme.begin()) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

}


void loop() {
  bmp_print();
  //Serial.println();//line break
  bh1750_print();
  am2322_print();
  Serial.println();//line break
  delay(1000);
}

