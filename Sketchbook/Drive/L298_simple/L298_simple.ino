

#define in1 9
#define in2 10


void setup() {

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);


}

void loop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(5000);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  delay(5000);
}
