#include <SoftwareSerial.h>
SoftwareSerial serial_test(3, 4);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  serial_test.begin(115200);
  delay(1000);
  Serial.println("Setup Complete!");
/*
  //test if the module is ready
  serial_test.println("AT+RST");
  delay(2000);
  if (serial_test.find("ready"))
  {
    Serial.println("Module is ready");
  }
  else
  {
    Serial.println("Module have no response.");
    while (1);
  }
  delay(1000);
  */
}


void loop() {


  if (serial_test.available()) {
    Serial.write(serial_test.read());
  }

  if (Serial.available()) {
    serial_test.write(Serial.read());
  }

}
