void setup() {

  // initialize digital pin LED_BUILTIN as an output.

  pinMode(LED_BUILTIN, OUTPUT);
  Serial2.swap(1);
  Serial2.begin(9600);

}

// the loop function runs over and over again forever

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial2.println("LED On");
  delay(250);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  Serial2.println("LED Off");
  delay(250);                       // wait for a second

}
