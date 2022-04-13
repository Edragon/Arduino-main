#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);

// EL125 connect 5V, GND, and TX pin to arduino D6

void setup() {
  Serial.begin(9600);
  while (!Serial);
  mySerial.begin(9600);

  delay(1000);
  
  Serial.println("software input pin D10");
  Serial.println("make sure the output selection pin is set to HIGH for serial output");
  Serial.println("Setup Complete!");

}

// command data type 02 30 31 31 33 35 38 38 33 44 33 1A 03 

void loop() {
  while (!mySerial.available()); // wait for the serial port to senr data

  int incomingByte = mySerial.read();

  Serial.print(incomingByte, HEX);

  if ( incomingByte == 0x03 ) { // the end byte of the tag ID
    Serial.println("");
  }

}
