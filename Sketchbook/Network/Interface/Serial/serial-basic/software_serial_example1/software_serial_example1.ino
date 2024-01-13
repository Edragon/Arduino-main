#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); // RX, TX

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //mySerial.listen();
  mySerial.println("Hello, world?");
  
  //mySerial.listen();
  
//  if (mySerial.available()) {
//    Serial.write(mySerial.read());
//  }
//  if (Serial.available()) {
//    mySerial.write(Serial.read());
//  }
  
  delay(100);
}
