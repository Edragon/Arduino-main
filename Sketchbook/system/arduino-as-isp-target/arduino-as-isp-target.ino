// arduino atmega328 = 1
// attiny2313 = 1， attinycore
// attiny24 = 9, attinycore

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(9, OUTPUT);
}


// the loop function runs over and over again forever
void loop() {
  digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
