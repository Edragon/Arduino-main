

#include <SoftwareSerial.h>

#define BAUD_RATE 4800


// for wemos 
// D5 is RX pin, D6 is TX pin
// HLW8032 TXD data pin should connect to D5
// Demo data should look like this 
// F2 5A 02 DA 78 07 1A E0 00 3D 3B 03 EC F5 4C C4 58 9C 2A 39 61 00 00 79 F2 5A 02 DA 78 07 1A E0 00 3D 3B 03 EC F5 4C C4 58 9D C2 9F 61 00 00 78 
// please read more details on wiki page

SoftwareSerial swSer(14, 12, false, 256);

void setup() {
  Serial.begin(BAUD_RATE);
  swSer.begin(BAUD_RATE);

  Serial.println("\nSoftware serial test started");

  for (char ch = ' '; ch <= 'z'; ch++) {
    swSer.write(ch);
  }
  swSer.println("");

}

void loop() {
  while (swSer.available() > 0) {
    Serial.write(swSer.read());
    yield();
  }
  while (Serial.available() > 0) {
    swSer.write(Serial.read());
    yield();
  }

}
