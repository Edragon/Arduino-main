// ESP-12F: DIO, 4M, 3M SPIFF
// 

void setup() {

  pinMode(15, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);

  digitalWrite(0, LOW);
  digitalWrite(2, LOW);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(15, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(15, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
