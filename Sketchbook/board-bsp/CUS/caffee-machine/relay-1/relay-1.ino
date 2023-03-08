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
}


void loop() {
  
  delay(2000);
  digitalWrite(relay, LOW);
  
  delay(2000);
  digitalWrite(relay, HIGH);


}
