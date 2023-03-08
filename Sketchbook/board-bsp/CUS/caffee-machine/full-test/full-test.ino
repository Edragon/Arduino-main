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
  pinMode(sense_input, INPUT);



  Serial.begin(9600);
  mySerial.begin(9600);

  digitalWrite(ac_detect, HIGH);
  digitalWrite(ac_detect2, HIGH);

  digitalWrite(GSM_BOOT, HIGH);
  digitalWrite(relay, LOW);
  delay(1000);
  digitalWrite(relay, HIGH);
  delay(1000);

  Serial.println ("ready.");

}

void loop() {
  if (digitalRead(ac_detect) == LOW || digitalRead(ac_detect2) == LOW ) {
    digitalWrite(relay, HIGH);
  }
  else {
    digitalWrite(relay, LOW);
  }

  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }

}
