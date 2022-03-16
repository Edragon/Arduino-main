#include <SoftwareSerial.h>.

#define GSM_BOOT  8
#define relay 6

SoftwareSerial mySerial(4, 5); // RX, TX

#define cmd1 "AT"
#define cmd2 "AT+BTPOWER=1"

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(GSM_BOOT, OUTPUT);
  digitalWrite(GSM_BOOT, HIGH);
  delay(5000);
  Serial.println ("ready.");

  if (TestCMD1(cmd1) != true) {
    GSM_reBOOT();
    Serial.println ("AT-NOK, re-power GSM");
  }


}


void loop() {

  ATtest();
}


void ATtest() {
  if (TestCMD1(cmd1) == true) {
    Serial.println("AT-OK, power is ON");
  }
  delay(1000);
  if (TestCMD1(cmd2) == true) {
    Serial.println("BT-OK, only turn ON once");
  }
  delay(1000);
}



bool TestCMD1 (String str) {
  mySerial.println(str);

  if (mySerial.find("OK")) {
    return true;
  } else {
    return false;
  }
}

void GSM_reBOOT() {

  digitalWrite(GSM_BOOT, LOW);
  delay(1000);
  digitalWrite(GSM_BOOT, HIGH);
  delay(5000);
  digitalWrite(GSM_BOOT, LOW);
  delay(1000);
}
