#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1); // RX, TX


void setup() {
  Serial2.begin(57600);

  mySerial.begin(57600);

}


void loop() {
  mySerial.println("test ...");
  delay(500);
  
}
