#include <SoftwareSerial.h>

//SoftwareSerial ms(5, 6); // Arduino RX, TX

SoftwareSerial bt(9, 7); // BT RX, TX

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(PB7, OUTPUT);
  pinMode(11, OUTPUT);
  
  Serial.begin(9600);
  bt.begin(9600);

}



void loop() {
  if (bt.available())
    Serial.write(bt.read());
    
  if (Serial.available())
    bt.write(Serial.read());
}
