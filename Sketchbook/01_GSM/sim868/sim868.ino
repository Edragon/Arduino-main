#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mySerial.begin(115200);

  pinMode(4, OUTPUT);

  GPS();
}

void loop() {
}

void GPS() {
  // put your main code here, to run repeatedly:
  digitalWrite(4, HIGH);    // turn on GPS
  delay(5000);                       // wait for a second

  mySerial.println("AT\r\n");
  delay(100);
  ShowSerialData();
  delay(1000);

  mySerial.println("AT+CGPSPWR=1\r\n");
  delay(100);
  ShowSerialData();
  delay(1000);

  mySerial.println("AT+CGPSRST=1\r\n");
  delay(100);
  ShowSerialData();
  delay(1000);
}

void ShowSerialData()
{
  while (mySerial.available() != 0)
    Serial.write(mySerial.read());
}
