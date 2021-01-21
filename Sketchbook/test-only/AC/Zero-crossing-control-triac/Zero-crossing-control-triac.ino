const byte ZCP = 2;
const unsigned int dim = 5000;  // 5000

unsigned long time;
void setup() {
  Serial.begin(9600);
  
  pinMode(ZCP, INPUT);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  digitalWrite(4, LOW);
  digitalWrite(5, LOW);

}
void loop() {
  //time = millis();
  if (time < 10000) {
    Serial.println("time in 10 secs");
    
    if (digitalRead(ZCP) == HIGH) {
      Zero_Cross();
    }
  }
  else {
  if (digitalRead(ZCP) == HIGH) {
      Zero_Cross();
    }
  }
}


void Zero_Cross() {
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delayMicroseconds(dim);

  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
}
