#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  pinMode(A7, OUTPUT);
  // put your setup code here, to run once:
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  mySerial.println("Hello, world?");
  digitalWrite(A7, HIGH);
  delay(1000);
  //  if (mySerial.available()) {
  //    Serial.write(mySerial.read());
  //  }
  //  if (Serial.available()) {
  //    mySerial.write(Serial.read());
  //  }

  digitalWrite(A7, LOW);
  delay(1000);
}
