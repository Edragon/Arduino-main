const int AnalogIn  = A0;

int readingIn = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  readingIn = analogRead(AnalogIn);
  Serial.println(readingIn);
}
