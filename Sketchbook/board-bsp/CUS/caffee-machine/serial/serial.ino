#include <SoftwareSerial.h>.

#define GSM_BOOT  8
#define relay 6


SoftwareSerial mySerial(4, 5); // RX, TX


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(GSM_BOOT, OUTPUT);
  digitalWrite(GSM_BOOT, HIGH);
}


void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }

  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
