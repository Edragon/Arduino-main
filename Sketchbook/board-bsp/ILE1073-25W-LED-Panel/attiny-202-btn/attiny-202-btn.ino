
#define IR 0
#define gnd_detect 3
#define btn 1
#define led 4

int buttonState = 0;  


void setup() {

  pinMode(btn, INPUT);
  pinMode(led, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  buttonState = digitalRead(btn);

  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(led, HIGH);
  } else {
    // turn LED off:
    digitalWrite(led, LOW);
  }
}
