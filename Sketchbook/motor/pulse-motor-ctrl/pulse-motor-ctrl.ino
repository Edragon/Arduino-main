

int pul = 9;           // the PWM pin the LED is attached to
int pwm_val = 100;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers



// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(pul, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(3, INPUT);

  digitalWrite(3,HIGH);
  
  digitalWrite(8,HIGH);
  
}


void loop() {
  int reading = digitalRead(3);

  if ( reading == LOW) {
    pwm_val = pwm_val + fadeAmount;
  }

  
  analogWrite(pul, pwm_val);

  if (pwm_val >= 255) {
    pwm_val = 0;
  }


  
}

void btn_bouncce() {


}

