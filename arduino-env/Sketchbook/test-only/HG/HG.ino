
// the setup function runs once when you press reset or power the board

int pwr = 2;
int btn = 9;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.

  pinMode(pwr, INPUT_PULLUP); // power input check
  pinMode(btn, INPUT_PULLUP); // power input check

  pinMode(10, OUTPUT); // led1
  pinMode(3, OUTPUT);  // led2
  //digitalWrite(2, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(3, HIGH);
}

// the loop function runs over and over again forever
void loop() {
  int power_in = digitalRead(2);
  int btn_in = digitalRead(9);


  if (power_in == 0 and btn_in == 0 ) {
    loop_led();
  }

}

void loop_led() {
  digitalWrite(10, HIGH);
  digitalWrite(3, LOW);
  delay(500);                       // wait for a second


  digitalWrite(10, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(3, HIGH);
  delay(500);                       // wait for a second

}
