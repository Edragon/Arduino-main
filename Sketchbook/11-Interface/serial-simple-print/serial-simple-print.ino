void setup() {
  Serial.begin(4800);
  Serial.println("Start, serial read and print in HEX");
}

void loop() {
  Serial.print("HELLO");
  delay(2000);
}
