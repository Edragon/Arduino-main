#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 4);

void setup() {
  Serial.begin(9600);
  Serial.println("beigin to start");
  
  mySerial.begin(9600);

  delay(1000);
}

void loop() {
  //mySerial.println("AT+CPOWD\r\n");
  mySerial.println("AT\r\n");
  delay(100);
  ShowSerialData();
  delay(1000);


}

void ShowSerialData()
{
  while ( mySerial.available() != 0 )
    Serial.write(mySerial.read());
}
