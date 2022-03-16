#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // D2 = RX, D3 = TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(1000);
  Serial.println("Setup Complete!");

}



void loop() {
    while(!mySerial.available()); // wait for the serial port to senr data
    
    int incomingByte = mySerial.read();
    
    Serial.println(incomingByte,HEX);
  }
