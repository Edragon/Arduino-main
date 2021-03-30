// read out hex data
// baudrate both must be 4800

// output data like:

// FA 5A 02 FB 48 FE FF 01 00 40 74 00 00 00 52 1B 88 F9 93 72 01 00 00 EB
// FA 5A 2  FB 48 FE FF  1  0 40 74 0  0   0 52 1B 88 FE DA 26  1 0   0 EB
//


byte hlw[23];

int flag = 0;

#include <SoftwareSerial.h>
#define BAUD_RATE 4800
SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  // must also be 4800

  Serial.begin(BAUD_RATE);
  mySerial.begin(BAUD_RATE);
}



void loop() {
  del();
  if (check() == true) {
    //Serial.print ("checked");
    rd2();
  }
}

void del() {
  for (int i = 1; i < 23; i++) {
    hlw[i] = 0;
  }
}


bool check() {
  while (1) {
    if (mySerial.available() > 0) {
      int rd = mySerial.read();

      if ( rd == 0xfa) {
        return true;
        break;
      }
    }
  }
}

void rd() {
  for (int i = 0; i < 23; i++) {
    if (mySerial.available() > 0) {
      hlw[i] = mySerial.read();
      Serial.write(hlw[i]);
    }
  }
}

void rd2() {
  //Serial.flush();
  
  //if (mySerial.available() > 0) {
    Serial.write(mySerial.read());
    Serial.write(mySerial.read());
    Serial.write(mySerial.read());
  //}

  
}
