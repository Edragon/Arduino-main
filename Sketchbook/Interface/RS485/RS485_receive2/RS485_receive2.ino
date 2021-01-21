void setup() {

  Serial.begin(9600);
  pinMode(8, OUTPUT);//DE/RE Controling pin of RS-485
}

void loop() {
  digitalWrite(8,LOW);//DE/RE=LOW Receive Enabled

}
