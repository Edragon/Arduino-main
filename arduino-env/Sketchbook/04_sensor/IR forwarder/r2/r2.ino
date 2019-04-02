//FA 65 26 9B 8B 80 80 02 00 87 9E 9B 8B 04 06 89 9D 87 9E 05 14 9C CA 89 9D 01 00 9A 8B 9C CA 02 00 88 9E 9A 8B 09 06 00 9B CA 88 9E 01 00 9B 8B 9B CB 02 00 88 9E 9B 8B 09 06 00 9A CA 88 9E 01 00 9C 8A 9A CA 02 00 88 9E 9C 8A 09 06 00 9B CA 88 9E 01 00 9B 8A 9B CA 02 00 87 9F 9B 8A 03 06 8A 9C 87 9F 06 04 1A 5D ED
//FA 6E 26 9B 8B 80 80 2 0 89 9D 9B 8B 96 09 BCA89 9D 1 0 9B 8B9BCA20899D9B8B9609BCA899D109C8A9BCA20899D9C8A9609BCA899D109B8A9BCB20889D9B8A9609BCA889D109B8A9BCB20889E9B8A9609CCA889E109B8A9CCB20889D9B8A9601A8FED

//#include <SoftwareSerial.h>

//SoftwareSerial mySerial(10, 9); // RX, TX

byte readdata[115];

boolean read_flag = true;

void cmd_read() {

  if (read_flag == true) {
    Serial.write(0xFA);
    Serial.write(0xF6);
    Serial.flush();
    delay(1000);
    Serial.write(0xFA);
    Serial.write(0xFD);
    read_flag = false;
  }
}


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("starting ..");


}

void loop() {
  cmd_read();

  if (Serial.available()) {
    //byte arrive_data;
    //arrive_data = Serial.read();
    for (int n = 0; n < 114; n++) {
      delay(3);
      readdata[n] = Serial.read();
    }

    if (readdata[0] == 0xFA) {
      Serial.println("FA - data received");
    }
    if (readdata[0] == 0x00) {
      Serial.println("00 - command confirmed");
    }
  }


  if (readdata[0] == 0xFA) {
    Serial.flush();
    //delay(1000);
    for (int j = 0; j < 114; j++) {
      //Serial.print(readdata[j], HEX);

      if ( readdata[j] == 255) {
        break;
      } else {
        Serial.print(readdata[j], HEX);
        //Serial.write(readdata[j]);
      }

    }
    //mySerial.write(0xED);
    Serial.println("");
    delay(100);
    if (Serial.available()) {
      Serial.println(Serial.read(), HEX);
    }
  }

  delay(1000);

}
