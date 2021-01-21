

void setup() {
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  
  Serial.begin(115200);
  
  Serial2.swap(1);
  Serial2.begin(115200);

}


void loop() {

  if (Serial2.available()) {
    Serial.write(Serial2.read());
  }
  if (Serial.available()) {
    Serial2.write(Serial.read());
  }
}
