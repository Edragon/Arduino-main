#include <SoftwareSerial.h>
SoftwareSerial mySerial(6, 7);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

}


void loop() {

  mySerial.println("AT\r\n");
  delay(100);
  ShowSerialData();
  delay(500);

  mySerial.println("AT+GSV\r\n");
  delay(100);
  ShowSerialData();
  delay(500);
  
  mySerial.println("AT+CSQ?\r\n");
  delay(100);
  ShowSerialData();
  delay(500);

  mySerial.println("AT+CPIN?\r\n");
  delay(100);
  ShowSerialData();
  delay(500);

  mySerial.println("AT+CREG?\r\n");
  delay(100);
  ShowSerialData();
  delay(500);

  mySerial.println("AT+CBAND=?\r\n");
  delay(100);
  ShowSerialData();
  delay(500);


  mySerial.println("AT+CENG?\r\n");
  delay(100);
  ShowSerialData();
  delay(500);
  
  mySerial.println("AT+CDNSGIP=www.163.com\r\n");  // www.163.com
  delay(100);
  ShowSerialData();
  delay(500);
  
  mySerial.println("AT+CIPPING=\"157.255.76.52\",1,32,100\r\n");  // www.163.com
  delay(100);
  ShowSerialData();
  delay(500);

  delay(3000);
}

void ShowSerialData()
{
  while (mySerial.available() != 0)
    Serial.write(mySerial.read());
}
