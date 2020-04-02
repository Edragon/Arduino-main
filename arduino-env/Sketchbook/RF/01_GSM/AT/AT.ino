#include <SoftwareSerial.h>

//SoftwareSerial mySerial(2, 3);
SoftwareSerial mySerial(2, 3); // new

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
  pinMode(8, OUTPUT); // boot GSM
  digitalWrite(8, HIGH);

}


void loop() {
  Serial.println("test START ...\r\n");
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
