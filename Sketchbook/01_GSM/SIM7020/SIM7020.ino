#include <SoftwareSerial.h>
SoftwareSerial mySerial(6, 7);

bool registered = false;


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

  mySerial.println("AT+CSQ?\r\n");   // useless for NBIOT, just let you know
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
  //ShowSerialData();
  flag_network();

  delay(500);

  if ( registered == true) {
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
}

void ShowSerialData()
{
  while (mySerial.available() != 0)
    Serial.write(mySerial.read());
}

void flag_network()
{
  while (mySerial.available() >= 20)
    if (mySerial.find("+CENG: ") ) {
      Serial.println("registered network! Make a DNS check and PING");
      registered = true;
    }

  if (mySerial.find("ERROR") ) {
    Serial.println("NOT registered network! Not play network");
    registered = false;
  }
}

