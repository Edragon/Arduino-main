/**************************************
* NewSoftSerial Example for
* The Arduino Mega/Mega 2560
*
* Limitations:
*
* You can only RECEIVE on pins:
* 10, 11, 12, 13,
* 50, 51, 52, 53,
* and 62, 63, 64, 65, 66, 67, 68, 69
*
***************************************/

#include <NewSoftSerial.h>

NewSoftSerial mySerial(50, 51);

void setup()  
{
  Serial.begin(57600);
  Serial.println("Goodnight moon!");

  // set the data rate for the NewSoftSerial port
  mySerial.begin(4800);
  mySerial.println("Hello, world?");
}

void loop()                     // run over and over again
{

  if (mySerial.available()) {
      Serial.print((char)mySerial.read());
  }
  if (Serial.available()) {
      mySerial.print((char)Serial.read());
  }
}
