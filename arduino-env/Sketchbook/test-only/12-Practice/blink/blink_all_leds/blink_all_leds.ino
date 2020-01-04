// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  for (int led = 18; led < 24; led++)
  pinMode(led, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {
  for (int led = 18; led < 24; led++){
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  delay(500);               // wait for a second
  for (int led = 18; led < 24; led++){
    digitalWrite(led, LOW); 
  }
  delay(500);               // wait for a second
}
