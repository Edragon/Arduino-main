#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(8, OUTPUT); // boot GSM
  digitalWrite(8, HIGH);

  pinMode(6, OUTPUT); // relay 1
  pinMode(7, OUTPUT); // relay 2

  Serial.begin(115200);
  mySerial.begin(115200);

}


void loop() {

  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
