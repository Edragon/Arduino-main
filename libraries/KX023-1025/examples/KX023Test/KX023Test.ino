/*
  KX023-1025 - Simple Accelerometer

  This example reads the acceleration values from the KX023-1025
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.

  created 22 Feb 2021
  by David Lyckelid

  This example code is in the public domain.
*/

#include <SPI.h>
#include <KX0231025IMU.h>

#define KX0231025_RANGE_2G       0
#define KX0231025_RANGE_4G       1
#define KX0231025_RANGE_8G       2

#define KX0231025_DATARATE_12_5HZ  0
#define KX0231025_DATARATE_25HZ    1
#define KX0231025_DATARATE_50HZ    2
#define KX0231025_DATARATE_100HZ   3
#define KX0231025_DATARATE_200HZ   4
#define KX0231025_DATARATE_400HZ   5
#define KX0231025_DATARATE_800HZ   6
#define KX0231025_DATARATE_1600HZ  7
#define KX0231025_DATARATE_0_781HZ   8
#define KX0231025_DATARATE_1_563HZ   9
#define KX0231025_DATARATE_3_125HZ   10
#define KX0231025_DATARATE_6_25HZ  11

#define KX0231025_LOWPOWER_MODE    0X00
#define KX0231025_HIGHPOWER_MODE   0X40

SPIClass hspi(HSPI);

KX0231025Class *imu;

void setup() {
  Serial.begin(9600);
  
  imu = new KX0231025Class(hspi,15);
  
  imu->begin(KX0231025_LOWPOWER_MODE, KX0231025_RANGE_8G, KX0231025_DATARATE_100HZ);
}


void loop() {
  float x, y, z;
  for(;;)
  {
      imu->readAcceleration(x, y, z);
      Serial.print(x);
      Serial.print('\t');
      Serial.print(y);
      Serial.print('\t');
      Serial.println(z);
  }
}
