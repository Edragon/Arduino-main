

void setup() {
  //pinMode(19, OUTPUT); 
  pinMode(15, OUTPUT); // 17 for ada bootloader or 0.17 for other firmware
  
  digitalWrite(15, HIGH); // low to enable 19 LED ON
}




// the loop function runs over and over again forever
void loop() {
  digitalWrite(15, HIGH);
  delay(200);
  digitalWrite(15, LOW);
  delay(200);
}
