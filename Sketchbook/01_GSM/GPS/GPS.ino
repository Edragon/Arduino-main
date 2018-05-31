#include <SoftwareSerial.h>
SoftwareSerial mySerial(6, 7);

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);

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
  
  delay(5000);
}

void ShowSerialData()
{
  while (mySerial.available() != 0)
    Serial.write(mySerial.read());
}
