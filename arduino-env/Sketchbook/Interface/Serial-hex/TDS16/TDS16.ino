#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX


int dat[5];
int datt1, datt2, datt3, datt4, datt5;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println("<Arduino is ready>");
}


void loop() {
  if (mySerial.available() > 0) {
    datt1 = mySerial.read();
    delay(2);
    datt2 = mySerial.read();
    delay(2);
    datt3 = mySerial.read();
    delay(2);
    datt4 = mySerial.read();
    delay(2);
    datt5 = mySerial.read();   
  }
  
  if (datt1 == 0xFF && datt2 == 0x00) {
    Serial.println(datt5);
  }
  delay(1000);
}
