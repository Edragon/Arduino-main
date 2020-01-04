#include <SoftwareSerial.h>

SoftwareSerial mySerial(6,7);

// EL125 connect 5V, GND, and TX pin to arduino D6

void setup() {
  Serial.begin(9600);
  while (!Serial);
  mySerial.begin(9600);
  delay(1000);
  Serial.println("Setup Complete!");

}



void loop() {
    while(!mySerial.available()); // wait for the serial port to senr data
    
    int incomingByte = mySerial.read();
    
    Serial.println(incomingByte,HEX);
  }
