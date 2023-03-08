#include <SoftwareSerial.h>.

#define GPRS_boot  8
#define relay 6

char rd[2];

SoftwareSerial mySerial(4, 5); // RX, TX


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  // boot
  //  pinMode(GPRS_boot, OUTPUT);
  //  digitalWrite(GPRS_boot, LOW);
  //  delay(3000);
  //  digitalWrite(GPRS_boot, HIGH);
  //  delay(3000);
  //  digitalWrite(GPRS_boot, LOW);

}


void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
    
//    if (mySerial.find("OK")) {
//      digitalWrite(relay, HIGH);
//    }
  }

  if (Serial.available()) {
    mySerial.write(Serial.read());
  }




}
