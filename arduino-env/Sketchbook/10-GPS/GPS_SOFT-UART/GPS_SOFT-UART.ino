char index = 0;
char temp = 0;
String dataString = "";

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() // run over and over
{
  while (mySerial.available())
  {
    temp = mySerial.read();
    dataString += String(temp);
    index++;
    if (index > 400)
      break;
  }
  Serial.print(dataString);
  delay(1000);
}
