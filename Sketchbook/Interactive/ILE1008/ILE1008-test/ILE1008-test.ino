/*
  Fading

  This example shows how to fade an LED using the analogWrite() function.

  The circuit:
  - LED attached from digital pin 9 to ground through 220 ohm resistor.

  created 1 Nov 2008
  by David A. Mellis
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Fading
*/

int b = 11;   
int r = 9;   
int g = 10;   


void setup() {
  // nothing happens in setup
}

void loop() {

  for (int fadeValue = 0 ; fadeValue <= 10; fadeValue += 1) {
    // sets the value (range from 0 to 255):
    analogWrite(r, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(50);
  }

  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 10 ; fadeValue >= 0; fadeValue -= 1) {
    // sets the value (range from 0 to 255):
    analogWrite(r, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(50);
  }

  for (int fadeValue = 0 ; fadeValue <= 10; fadeValue += 1) {
    // sets the value (range from 0 to 255):
    analogWrite(g, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(50);
  }

  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 10 ; fadeValue >= 0; fadeValue -= 1) {
    // sets the value (range from 0 to 255):
    analogWrite(g, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(50);
  }

    for (int fadeValue = 0 ; fadeValue <= 10; fadeValue += 1) {
    // sets the value (range from 0 to 255):
    analogWrite(b, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(50);
  }

  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 10 ; fadeValue >= 0; fadeValue -= 1) {
    // sets the value (range from 0 to 255):
    analogWrite(b, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(50);
  }

  
}
