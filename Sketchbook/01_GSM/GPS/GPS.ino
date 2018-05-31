#include <SoftwareSerial.h>
SoftwareSerial mySerial(6, 7);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  delay(5000);
}


void loop() {

  

  mySerial.println("AT+CGPSPWR=1\r\n");
  delay(100);
  ShowSerialData();
  
  delay(1000);
  
  mySerial.println("AT+CGNSPWR=1\r\n");
  delay(100);
  ShowSerialData(); 

  delay(1000);
  mySerial.println("AT+CGPSPWR?\r\n");
  delay(100);
  ShowSerialData();
  
  delay(1000);
  
  mySerial.println("AT+CGNSPWR?\r\n");
  delay(100);
  ShowSerialData(); 
  
  mySerial.println("AT+CREG?\r\n");
  delay(100);
  ShowSerialData();  

  mySerial.println("AT+CPIN?\r\n");
  delay(100);
  ShowSerialData();  
  
  delay(10000);
}

void ShowSerialData()
{
  while (mySerial.available() != 0)
    Serial.write(mySerial.read());
}
