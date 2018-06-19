#include <SoftwareSerial.h>
SoftwareSerial serial_test(6,7);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  serial_test.begin(115200);
  delay(1000);
  Serial.println("Setup Complete!");
  //serial_test.println("AT+RST");
}


void loop() {
  

  if (serial_test.available()) {
    Serial.write(serial_test.read());
  }

  if (Serial.available()) {
    serial_test.write(Serial.read());
  }

}
