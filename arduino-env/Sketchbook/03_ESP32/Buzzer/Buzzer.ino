
// frequency buzzer 

int beeper = 5; // buzzer IO 5

void setup() {

  ledcAttachPin(beeper, 3); // attach to channel 3
  ledcSetup(3, 800, 8); // channel 3, frequency 500kzh, 8 bit
}




void loop() {

  //ledcWrite(3, 0);
  //delay(1000);

  for (int i = 0; i <= 255; i++) {
    ledcWrite(3, i); // dutycycle
  }
  delay(1000);
}
