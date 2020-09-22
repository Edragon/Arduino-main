void setup() {
  Serial.begin(57600);  // D0 D1


  Serial2.swap(1);  // pin 24 25
  Serial2.begin(57600);  // D0 D1
  
  pinMode(7, OUTPUT);
  digitalWrite(7,HIGH);
  
  Serial.println("AT\r\n");
  Serial2.println("AT\r\n");
}


void loop() {

  if (Serial.available()) {
    Serial2.write(Serial.read());
  }
  if (Serial2.available()) {
    Serial.write(Serial2.read());
  }
}
