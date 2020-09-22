

void setup() {
  Serial.begin(57600);  // D0 D1


  Serial2.swap(1);  // pin 24 25
  Serial2.begin(57600);  // D0 D1
  pinMode(7, OUTPUT);

}


void loop() {
  Serial.println("AT\r\n");
  
  digitalWrite(7, HIGH);
  
  if (Serial.find("OK"))
  { Serial2.println("receive OK!");
  }
  delay(500);

  
  digitalWrite(7, LOW);
  delay(500);
}
