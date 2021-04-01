// read out hex data
// baudrate both must be 4800

// output data like:

// FA 5A 02 FB 48 FE FF 01 00 40 74 00 00 00 52 1B 88 F9 93 72 01 00 00 EB
// FA 5A 2  FB 48 FE FF  1  0 40 74 0  0   0 52 1B 88 FE DA 26  1 0   0 EB
// 
const byte numBytes = 24;
byte hlw[numBytes];

bool print_flag = false;

#include <SoftwareSerial.h>
#define BAUD_RATE 4800
SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  // must also be 4800

  Serial.begin(BAUD_RATE);

  mySerial.begin(BAUD_RATE);
}


void prt_dat_bk1() {
  static byte pt = 0;
  while (pt <= numBytes) {
    Serial.write(hlw[pt]);
    pt++;
  }
  //Serial.println(" ");
}


void prt_dat_test() {
  Serial.write(hlw[0]);
  }
  
void prt_dat() {
  
  if (hlw[0] == 0x5A && hlw[1] != 0x00 && hlw[2] != 0x00 && hlw[3] != 0x00 && hlw[22] != 0x00) {
    for (int i = 0; i <= 24; i++) {
      Serial.write(hlw[i]);
    }
  }

}

void parse_3dat(int a, int b, int c) {

  Serial.print(hlw[a], HEX);
  Serial.print(" ");
  Serial.print(hlw[b], HEX);
  Serial.print(" ");
  Serial.print(hlw[b], HEX);
  Serial.print(" ");
  Serial.println("");
}

void format_print() {

}



void del() {
  for (int i = 0; i < 25; i++) {
    hlw[i] = 0;
  }
}


bool rec() {
  static boolean recvInProgress = false;
  byte startMarker = 0xFA;
  byte rb;
  static byte ndx = 0;

  while (mySerial.available() > 0) {
    //delay(5);
    rb = mySerial.read();

    if (recvInProgress == true) {
      if (ndx <= numBytes) {
        hlw[ndx] = rb;
        //Serial.write(hlw[ndx]);
        ndx++;
      }
      else {
        ndx = 0;
        recvInProgress = false;

      }
    }

    if (rb == startMarker) {
      del();
      recvInProgress = true;
      //Serial.println("find2");
    }
  }

}




void loop() {
  rec();
  prt_dat();
  //prt_dat_test();
  //parse_3dat(1, 2, 3);
  //del();
}
