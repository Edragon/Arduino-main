// read out hex data
// baudrate both must be 4800

// output data like:

// FA 5A 02 FB 48 FE FF 01 00 40 74 00 00 00 52 1B 88 F9 93 72 01 00 00 EB
// FA 5A 2  FB 48 FE FF  1  0 40 74 0  0   0 52 1B 88 FE DA 26  1 0   0 EB
//

const byte numBytes = 24;
byte hlw[numBytes];




#include <SoftwareSerial.h>
#define BAUD_RATE 4800
SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  // must also be 4800

  Serial.begin(BAUD_RATE);

  mySerial.begin(BAUD_RATE);
}



void loop() {
  rec();
  //showNewData();

}

void del() {
  for (int i = 1; i < 23; i++) {
    hlw[i] = 0;
  }
}


bool rec() {
  static boolean recvInProgress = false;
  byte startMarker = 0xFA;
  byte rb;
  static byte ndx = 0;

  while (mySerial.available() > 0) {
    rb = mySerial.read();

    if (recvInProgress == true) {
      if (ndx <= numBytes) {
        hlw[ndx] = rb;
        Serial.write(hlw[ndx]);
        ndx++;
      }
      else {
        ndx = 0;
        recvInProgress = false;
        
      }
    }
    if (rb == startMarker) {
      recvInProgress = true;
      Serial.println("find2");
    }

  }
}


