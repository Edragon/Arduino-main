#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println("<Arduino is ready>");
}

void loop() {
  int data1 = mySerial.read();
  Serial.println(data1, HEX);
}
