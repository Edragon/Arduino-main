#include <SoftwareSerial.h>

SoftwareSerial mySerial(6, 7); // RX, TX



// demo data from serial
// F2 5A 02 E0 B8  07 34 2D 00 3E  67 07 22 DC 4D  ED 38 DF EC 33  61 00 00 7D
void setup() {
  Serial.begin(4800); // HLW8032
  
  mySerial.begin(4800);
  
}

void loop() {
  String inputString = "";
  if(Serial.available() >0) {
    for (int length = 0; length < 24; length ++) {
      //delay(20);
      char inChar = (char)Serial.read();
      inputString += inChar;
      //buffer[length] = f;
    }
  }
  
  //buffer[23] = '\0';
  mySerial.print( inputString.substring(0, 24) );
  delay (1000);
}
