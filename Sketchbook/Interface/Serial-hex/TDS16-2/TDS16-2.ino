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

    for (int i = 0; i < 5; i++) {
      dat[i] = mySerial.read();
    }
  }

  if (dat[0] == 0xFF && dat[1] == 0x00) {
    //Serial.println(dat[3]);
    Serial.println(dat[4]);
  }

  delay(1000);



}
