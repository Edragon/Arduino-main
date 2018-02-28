//FA 65 26 9B 8B 80 80 02 00 87 9E 9B 8B 04 06 89 9D 87 9E 05 14 9C CA 89 9D 01 00 9A 8B 9C CA 02 00 88 9E 9A 8B 09 06 00 9B CA 88 9E 01 00 9B 8B 9B CB 02 00 88 9E 9B 8B 09 06 00 9A CA 88 9E 01 00 9C 8A 9A CA 02 00 88 9E 9C 8A 09 06 00 9B CA 88 9E 01 00 9B 8A 9B CA 02 00 87 9F 9B 8A 03 06 8A 9C 87 9F 06 04 1A 5D ED
//FA 6E 26 9B 8B 80 80 2 0 89 9D 9B 8B 96 09 BCA89 9D 1 0 9B 8B9BCA20899D9B8B9609BCA899D109C8A9BCA20899D9C8A9609BCA899D109B8A9BCB20889D9B8A9609BCA889D109B8A9BCB20889E9B8A9609CCA889E109B8A9CCB20889D9B8A9601A8FED


// to modify softwareserial, in arduino folder -> \arduino-1.8.3\hardware\arduino\avr\libraries\SoftwareSerial\src
// file softwareserial.h, change definition:
// #define _SS_MAX_RX_BUFF 128 // RX buffer size
// change to 128

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 9); // RX, TX

byte readdata[200];
byte uartStatus;

boolean read_flag = true;

void cmd_read() {

  if (read_flag == true) {
    mySerial.write(0xFA);
    mySerial.write(0xF6);
    mySerial.flush();
    delay(1000);
    mySerial.write(0xFA);
    mySerial.write(0xFD);
    read_flag = false;
  }
}


void setup() {
  //mySerial.setTimeout(2000);
  for (int n = 0; n < 200; n++) {
    readdata[n] = 0xFF;
  }
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("starting ..");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

}

void loop() {
  cmd_read();
  int pos = 0;
  delay(100);
  if (mySerial.available() < 2) {
    uartStatus = mySerial.read();
  }
  while (mySerial.available() > 0) {
    //readdata[pos++] = mySerial.read();
    mySerial.readBytes(readdata, 120);
  }

  if (readdata[0] == 0xFA) {
    Serial.println("FA - data received");
  }
  if (uartStatus == 0x00) {
    Serial.println("00 - command confirmed");
  }



  if (readdata[0] == 0xFA) {
    mySerial.flush();
    //delay(1000);
    for (int j = 0; j < 199; j++) {
      //Serial.print(readdata[j], HEX);

      if ( readdata[j] == 0xFF) {
        break;
      } else {
        Serial.print(readdata[j], HEX);
        mySerial.write(readdata[j]);
      }

    }
    mySerial.flush();
    //mySerial.write(0xED);
    Serial.println("");
    delay(100);
    if (mySerial.available()) {
      Serial.println(mySerial.read(), HEX);
    }
  }

  delay(1000);

}
