#include <SoftwareSerial.h>
SoftwareSerial BT(10, 9); // RX, TX

void setup() {
  Serial.begin(9600);
  BT.begin(115200);
}

void loop() {
  if (Serial.available()) {
    BT.write(Serial.read());
  }
  if (BT.available()) {
    Serial.write(BT.read());
  }
}
