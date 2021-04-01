// read out hex data
// baudrate both must be 4800

// output data like, total length 24 bytes:

// FA 5A 02 FB 48 FE FF 01 00 40 74 00 00 00 52 1B 88 F9 93 72 01 00 00 EB

int hlw[23];

#include <SoftwareSerial.h>
#define BAUD_RATE 4800

SoftwareSerial mySerial(10, 11); // RX, TX  // HLW8032 TX to D10

void setup() {
  // must also be 4800
  Serial.begin(BAUD_RATE);
  mySerial.begin(BAUD_RATE);

}



void fil_dat() {
  //  int dat = mySerial.read();
  //  delay(20);
  //
  //  if (dat == 0xFA) {
  //
  //    int dat2 = mySerial.read();
  //
  //    if (dat2 == 0x5A) {
  //      hlw[0] = 0xfa;
  //      hlw[1] = 0x5a;
  //
  
  for (int i = 0; i < 23; i++) {
    int incomingByte = mySerial.read();
    //delay(20);
    hlw[i] = incomingByte;
  }


}



void clear_dat () {
  for (int i = 1; i < 23; i++) {
    hlw[i] = 0;
  }
}


void loop() {
  clear_dat();
  fil_dat();
  Serial.println();

  for (int i = 0; i < 23; i++) {
    Serial.print(hlw[i], HEX);
    Serial.print(" ");
  }


}
