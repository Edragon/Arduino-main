#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX


int dat[2];

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println("<Arduino is ready>");
}


void loop() {
  if (mySerial.available() > 0) {

    for (int i = 0; i < 2; i++) {
      dat[i] = mySerial.read();
    }
  }

  if (dat[0] == 0xFF && dat[1] == 0x00) {
    //Serial.println(dat[3]);
    Serial.println(dat[0]);
    Serial.println(dat[1]);
  }

}
