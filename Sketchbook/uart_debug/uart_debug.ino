#include <SoftwareSerial.h>
SoftwareSerial serial_test(2, 3);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  serial_test.begin(9600);
  delay(1000);
  Serial.println("Setup Complete!");
}


void loop() {
  if (serial_test.available()) {
    Serial.write(serial_test.read());
  }
  
  if (Serial.available()) {
    serial_test.write(Serial.read());
  }
}
