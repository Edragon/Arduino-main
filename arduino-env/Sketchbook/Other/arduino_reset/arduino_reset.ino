int led = 6;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  Serial.begin(9600);//initialize the serial port
}
void(* resetFunc) (void) = 0; //declare reset function @ address 0


// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println("on");
  delay(1000);               // wait for a second

  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("off");
  delay(1000);               // wait for a second
  Serial.println("resetting");
  resetFunc();  //call reset

  delay(100);
  Serial.println("never happens");
}
