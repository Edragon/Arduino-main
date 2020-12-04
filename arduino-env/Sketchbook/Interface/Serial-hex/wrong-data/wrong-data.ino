#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX



void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println("<Arduino is ready>");
}



void loop() {
  
  int datt1 = mySerial.read();
  if (datt1 == 0xFFFFFFFF ) {
    Serial.println("wrong data");
  } else {
    Serial.println("correct data");
    }


}
