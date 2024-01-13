
#include <RHReliableDatagram.h>
#include <RH_RF95.h>

//get it here: https://github.com/LowPowerLab/SPIFlash
// this library conflict with other same name
#include <SPIFlash.h>
#include <SPI.h>

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// 0x1F44 for adesto(ex atmel) 4mbit flash
// 0xEF30 for windbond 4mbit flash
// 0xEF40 for windbond 64mbit flash

SPIFlash flash(8, 0xEF40); // 25Q16

RH_RF95 driver(10, 2);

RHReliableDatagram manager(driver, CLIENT_ADDRESS);

void volt_read()
{
  char data_volt[80];
  char pre_volt[] = "this board battery level is: ";
  char volt_char[5];

  float volt = analogRead(A7);
  volt = volt / 217 ;            // calculate battery, linear
  dtostrf(volt, 6, 2, volt_char);         // data to string

  sprintf(data_volt, "%s %s", pre_volt, volt_char);   // combine char array

  Serial.println(data_volt);
  delay(100);        // delay in between reads for stability

}

void setup()
{
  // Rocket Scream Mini Ultra Pro with the RFM95W only:
  // Ensure serial flash is not interfering with radio communication on SPI bus
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);

  Serial.begin(9600);

  while (!Serial) ; // Wait for serial port to be available
  if (!manager.init())
    Serial.println("...RF Init FAIL!");
  else
    Serial.println("...RF Init OK!");
    
  driver.setFrequency(434.0);
  Serial.println(".BAND Set To 434.0 MHz");
  
  // initialize Flash
  if (flash.initialize())
  {
    Serial.println("Flash Init OK!");
  }
  else
    Serial.println("Flash Init FAIL!");

  delay(1000);
}


uint8_t data[] = "Hello World!";
// Dont put this on the stack:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];



void loop()
{
  volt_read(); // data_volt

  Serial.println("Sending to rf95_reliable_datagram_server");

  // Send a message to manager_server
  if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS))
  {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
    { digitalWrite(7, LOW);
      delay(500);
      digitalWrite(7, HIGH);

      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);
    }
    else
    {
      Serial.println("No reply, is rf95_reliable_datagram_server running?");
    }
  }
  else
    Serial.println("sendtoWait failed");
  delay(1000);
}
