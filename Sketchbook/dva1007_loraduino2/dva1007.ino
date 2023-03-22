#include "LowPower.h"
#include <SPIFlash_lp.h>    //get it here: https://github.com/LowPowerLab/SPIFlash
#include <SPI.h>
#include <RHReliableDatagram.h>
#include <RH_RF95.h>

#define SERIAL_BAUD      115200

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

#define LED           7 // Moteinos have LEDs on D9
#define FLASH_SS      8 // and FLASH SS on D8


SPIFlash flash(FLASH_SS, 0xEF40);

// Singleton instance of the radio driver
RH_RF95 driver(10, 2);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, CLIENT_ADDRESS);


uint8_t data[] = "Hello World!";
// Dont put this on the stack:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

void setup() {
  // init uart
  Serial.begin(SERIAL_BAUD);
  Serial.print("Start...");

  // init RF
  //while (!Serial) ; // Wait for serial port to be available
  if (manager.init())
  {
    Serial.println("RF init OK");
    Blink(LED, 100, 1);
  }
  else
  {
    Serial.println("RF init fail!");
  }
  driver.setFrequency(434.0);
  delay(1000);

  if (driver.sleep())
  {
    Serial.println("RF sleep OK");
    Blink(LED, 50, 5);
  } else {
    Serial.println("RF sleep fail!");
  }


  // init flash


  if (flash.initialize())
  {
    Serial.println("Flash Init OK!");
    Blink(LED, 20, 10);
  }
  else
    Serial.println("Flash Init FAIL!");
  delay(1000);

  flash.sleep();
  Serial.println("Flash should be slept");

}



void loop() {

  Blink(LED, 20, 30);
  Serial.println("loop blink");
  delay(1000);

  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}





void Blink(byte PIN, int DELAY_MS, byte loops)
{
  pinMode(PIN, OUTPUT);
  while (loops--)
  {
    digitalWrite(PIN, HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN, LOW);
    delay(DELAY_MS);
  }
}
