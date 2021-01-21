#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX
#define GPRS_boot  8

void setup() {
  Serial.begin(19200);
  mySerial.begin(19200);

  // boot
  pinMode(GPRS_boot, OUTPUT);
  digitalWrite(GPRS_boot, LOW);
  delay(3000);
  digitalWrite(GPRS_boot, HIGH);
  delay(3000);
  digitalWrite(GPRS_boot, LOW);

}


void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }

}
