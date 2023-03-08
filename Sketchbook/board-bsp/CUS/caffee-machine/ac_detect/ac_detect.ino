#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX

#define ac_detect 2
#define ac_detect2 3
#define relay 6
#define GSM_BOOT 8
#define sense_input 13


void setup() {
  pinMode(relay, OUTPUT);

  pinMode(GSM_BOOT, OUTPUT);

  pinMode(ac_detect, INPUT);
  pinMode(ac_detect2, INPUT);
  digitalWrite(ac_detect, HIGH);
  digitalWrite(ac_detect2, HIGH);
  
  pinMode(sense_input, INPUT);
  
  digitalWrite(GSM_BOOT, HIGH);

  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println ("ready.");

}

void loop() {

  // serial 
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }

  // ac detect input
  if (digitalRead(ac_detect) == LOW || digitalRead(ac_detect2) == LOW ) {
  //if (digitalRead(ac_detect) == LOW ) {
    digitalWrite (relay, HIGH);
  }
  else {
    digitalWrite (relay, LOW);
  }

  
  //socket input
  if (digitalRead(sense_input) == HIGH) {
    digitalWrite (relay, HIGH);
  }
  else {
    digitalWrite (relay, LOW);
  }

}
