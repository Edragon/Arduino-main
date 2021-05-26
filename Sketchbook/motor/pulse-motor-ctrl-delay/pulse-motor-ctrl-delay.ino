

int pul = 9;           // pulse
int dir = 8;           // pulse

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(pul, OUTPUT);
  pinMode(dir, OUTPUT);
  digitalWrite(dir, HIGH);

}


void loop() {
    digitalWrite(9, HIGH);
    delayMicroseconds(500);
    digitalWrite(9, LOW);
    delayMicroseconds(500);
}


