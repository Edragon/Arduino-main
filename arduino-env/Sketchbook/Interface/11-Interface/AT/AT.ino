#include <SoftwareSerial.h>
SoftwareSerial mySerial(6, 7);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  delay(1000);
}


void loop() {

  delay(1000);

  mySerial.println("ATI\r\n");
  delay(100);

  ShowSerialData();
    
}

void ShowSerialData()
{
  while (mySerial.available() != 0)
    Serial.write(mySerial.read());
}
