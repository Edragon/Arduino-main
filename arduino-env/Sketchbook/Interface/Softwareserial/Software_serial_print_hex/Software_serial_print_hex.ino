#include <SoftwareSerial.h>

SoftwareSerial mySerial(6,7);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  mySerial.begin(9600);
  delay(1000);
  Serial.println("Setup Complete!");

}


void p(char X) {

   if (X < 16) {Serial.print("0");}

   Serial.println(X, HEX);

}

void loop() {
    while(!mySerial.available()); // wait for the serial port to senr data
    int incomingByte = mySerial.read();
    Serial.println(incomingByte,HEX);
  }
