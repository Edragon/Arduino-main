#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);


void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);

}


void loop() {

  mySerial.println("AT\r\n");
  delay(100);
  ShowSerialData();
  delay(3000);

}

void ShowSerialData()
{
  while (mySerial.available() != 0)
    Serial.write(mySerial.read());
}

