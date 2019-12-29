void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serial.println("\ntest\n");
}

void loop() {
  Serial.println("\n turn off \n");
  digitalWrite(3, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(13, LOW);
  delay(5000);

  // set high to turn off motor
  Serial.println("\n turn on \n");
  digitalWrite(3, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(13, HIGH);
  delay(5000);
}
