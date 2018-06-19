#include <SoftwareSerial.h>
SoftwareSerial mySerial(6, 7);

String ATI = "ATI\r\n";
String ATE = "ATE=1\r\n";
String CGMI = "AT+CGMI\r\n";
String CGMM = "AT+CGMM\r\n";
String CSCON = "AT+CSCON?\r\n";
String CEREG = "AT+CEREG?\r\n";
String CGPADDR = "AT+CGPADDR?\r\n";
String NCCID = "AT+NCCID\r\n";
//AT+NATSPEED=9600,3,1,0,1
String NATSPEED = "ATNATSPEED?\r\n";

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  delay(1000);
}

void loop() {
  send_AT(ATI);
  send_AT(ATE);
  send_AT(CGMI);
  send_AT(CGMM);
  send_AT(CSCON);
  send_AT(CEREG);
  send_AT(CGPADDR);
  send_AT(NCCID);
  send_AT(NATSPEED);
}

void send_AT(String commands) {

  delay(1000);
  mySerial.println(commands);
  //mySerial.println("");
  //Serial.flush();
  delay(100);
  Serial.println("-----------");
  ShowSerialData();
}

void ShowSerialData()
{
  while (mySerial.available() != 0)
    Serial.write(mySerial.read());
}
