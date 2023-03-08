int led1 = 9;           // the PWM pin the LED is attached to
int led2 = 10;           // the PWM pin the LED is attached to
int led3 = 11;           // the PWM pin the LED is attached to


void setup() {
  // declare pin 9 to be an output:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}


void loop(){
  led_loop(led1);
  led_loop(led2);
  led_loop(led3);
  }


void led_loop(int ledx) {
  // fade in from min to max in increments of 5 points:
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    analogWrite(ledx, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(10);
  }

  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    // sets the value (range from 0 to 255):
    analogWrite(ledx, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(10);
  }
}
