void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);//DE/RE Controling pin of RS-485
  digitalWrite(8,HIGH);//DE/RE=HIGH Transmit Enabled M1
}

void loop() {
  Serial.print('9');//Write '9' and Fetch Data From Pro Mini
 
}
