

void setup() {
  pinMode(19, OUTPUT); 
  pinMode(17, OUTPUT); // 17 for ada bootloader or 0.17 for other firmware
  
  digitalWrite(19, HIGH);
}




// the loop function runs over and over again forever
void loop() {
  digitalWrite(17, HIGH);
  delay(200);
  digitalWrite(17, LOW);
  delay(200);
}
