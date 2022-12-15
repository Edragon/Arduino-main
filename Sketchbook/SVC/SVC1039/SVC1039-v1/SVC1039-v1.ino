
#define relay 1
#define led 0 
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(relay, OUTPUT);
  pinMode(led, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(relay, HIGH);
  digitalWrite(led, LOW);
  delay(1000);
  
  digitalWrite(relay, LOW);
  digitalWrite(led, HIGH);
  delay(1000);
}
