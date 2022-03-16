#include <SoftwareSerial.h>

int inBuffer[3];
byte bufIndx = 0;

SoftwareSerial mySerial(4, 5);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  mySerial.begin(9600);
  delay(1000);
  Serial.println("Setup Complete!");

}

void loop() {
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
  
  bufIndx = 0;
  while (!mySerial.available()); // wait for the serial port to send data


  if (mySerial.available() > 0) {
    Serial.println();
    while (mySerial.available() > 0) {
      inBuffer[bufIndx] = mySerial.read();
      bufIndx ++;
    }
    inBuffer[bufIndx] = 0;
  }
  for (int i = 0; i < 3; i++) Serial.println (inBuffer[i], DEC);


}
