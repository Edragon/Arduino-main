

void send_data() {
  //digitalWrite(11, LOW); // wake up in low power mode
  // send byte
  //ms.listen();
  while (Serial.available() > 0) {
    char inByte = Serial.read();
    bt.write(inByte);
  }

  //delay(10);

}

void get_data() {
  bt.listen();
  while (bt.available() > 0) {
    char inByte = bt.read();
    Serial.write(inByte);
  }
}

void simple_write() {
  // simple write method
  //  bt.println("987654");
  //  ms.println("987654");
  //  delay(1000);
}


void AT1() {
  //digitalWrite(11, LOW); // wake up in low power mode
  // send byte
  Serial.println("AT+MODE=0");
  delay(10);
  
  Serial.println("AT+NAME=HAPPY_CUSTOMER");
  delay(10);
  delay(10000);
}
