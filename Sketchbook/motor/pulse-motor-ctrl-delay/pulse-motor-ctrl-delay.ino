

int pul = 9;           // the PWM pin the LED is attached to
int pwm_val = 0;    // how bright the LED is
int fadeAmount = 1;    // how many points to fade the LED by


// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(pul, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);

}


void loop() {
    digitalWrite(9, HIGH);
    delayMicroseconds(100);
    digitalWrite(9, LOW);
}

void btn_bouncce() {


}

