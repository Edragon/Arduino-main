#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

// Jumper D2 top jumper D3 bottom


void setup() {
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  Serial.begin(9600);
  
  mySerial.begin(9600);

}


void loop() {

  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
