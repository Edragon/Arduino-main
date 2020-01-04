void listen() {
  delay(100);
  mySerial.write(0xFA);
  mySerial.write(0xFD);
  delay(100);
}

void quit() {
  delay(100);
  mySerial.write(0xFA);
  mySerial.write(0xF4);
  delay(100);
}


void FA_buffer() {
  
  int cnt2 = 0;
  if (mySerial.available() > 0) {
    delay(10);
    for (int cnt = 0; cnt < 200; cnt++) {
      ary_get[cnt] = mySerial.read();
      if (ary_get[cnt] == 0) {
        Serial.println("get 00 ..");
      } else if (ary_get[cnt] == 0xFA) {
        Serial.println("get FA ..");
      }
      
      /*if (check == true) {
        ary_sort[cnt2] = ary_get[cnt];
        cnt2++;
      }*/
    }
  }
}

void cmd_buffer() {
  
  //int cnt2 = 0;
  if (Serial.available() > 0) {
    delay(10);
    for (int cnt = 0; cnt < 6; cnt++) {
      unsigned char data_get = Serial.read();
      if (data_get = 0x00) {
        Serial.print(data_get);
        //Serial.flush();
      }
    }
  }
}

void print_get() {
  Serial.println("print char array get");
  for (int j = 0; j < 200; j++) {
    Serial.print(ary_get[j], HEX);
  }
  Serial.println("");
}

void print_sort() {
  if (check = true) {
    Serial.println("detect header ..");
    
  }
  Serial.println("print char array sort");
  Serial.flush();
  for (int j = 0; j < 120; j++) {
    Serial.print(ary_sort[j], HEX);
  }
  Serial.flush();
  Serial.println("");
  Serial.flush();
}

/*void sendhex() {
  delay(100);
  mySerial.write(ary_get, 200);
  delay(100);
}*/

