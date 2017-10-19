void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() 
{
  digitalWrite(0, HIGH);       // turn the LED on (HIGH is the voltage level)
  digitalWrite(1, HIGH);
  digitalWrite(2, HIGH);
  delay(100);                       // wait for 100ms
  digitalWrite(0, LOW);       // turn the LED off by making the voltage LOW
  digitalWrite(1, LOW);
  digitalWrite(2, LOW);
  delay(100);                       // wait for a 0,5 seconds
}
