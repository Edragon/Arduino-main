const byte ZCP = 2;
const unsigned int dim = 5000;  // 5000
void setup() {
  pinMode(ZCP, INPUT);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  digitalWrite(4, LOW);
  digitalWrite(5, LOW);

}
void loop() {
  if (digitalRead(ZCP) == LOW) {
    Zero_Cross();
  }
}

void Zero_Cross() {
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delayMicroseconds(dim);

  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
}
