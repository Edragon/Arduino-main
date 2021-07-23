
void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

}


void loop() {
  digitalWrite(3, LOW);
  delay(100);
  digitalWrite(3, HIGH);
  delay(3000);
}
