/*
 * Basic IO
 * Use a pushbutton to toggle the onboard LED.
 *
 * If you do not have the 1 Button Shield, add a pushbutton or switch between D3 and GND
 */

//int inputPin = D3;  // pushbutton connected to digital pin D3
int val = 1;        // variable to store the read value

void setup() {
  pinMode(D0, OUTPUT);  // set onboard LED as output
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
 // pinMode(D9, OUTPUT);
 // pinMode(D10, OUTPUT);
  
  //pinMode(inputPin, INPUT);      // set pin as input
}

void loop() {
  val = 0;//digitalRead(inputPin);     // read the input pin
  digitalWrite(D0, val);  // sets the LED to the button's value
  digitalWrite(D1, val);
  digitalWrite(D2, val);
  digitalWrite(D3, val);
  digitalWrite(D4, val);
  digitalWrite(D5, val);
  digitalWrite(D6, val);
  digitalWrite(D7, val);
  digitalWrite(D8, val);
  //digitalWrite(D9, val);
  //digitalWrite(D10, val);
}
