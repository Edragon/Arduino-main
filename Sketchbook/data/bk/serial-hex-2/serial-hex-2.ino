#include <SoftwareSerial.h>

SoftwareSerial mySerial(6, 7); // RX, TX


String CharArrayInLocal[20];
String inputString = "";

boolean stringComplete = false;
int i = 0;

void serialEvent() {
  
  while (Serial.available()) {

    char inChar = (char)Serial.read();

    if (inChar == '\n') {

      CharArrayInLocal[i] = inputString;

      i++;
      stringComplete = true;
      inputString = "";
    }
    else {
      inputString += inChar;
    }
  }
  mySerial.print( inputString );
  delay (1000);
}
