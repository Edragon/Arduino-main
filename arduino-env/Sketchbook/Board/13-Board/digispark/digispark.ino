void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(1, OUTPUT);

}

// the loop function runs over and over again forever
void loop() 
{
  digitalWrite(1, HIGH);
  delay(100);    // wait for 100ms
  
  digitalWrite(1, LOW);
  delay(100);                       // wait for a 0,5 seconds
}
