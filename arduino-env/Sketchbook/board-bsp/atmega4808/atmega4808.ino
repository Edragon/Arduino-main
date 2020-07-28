
// the setup function runs once when you press reset or power the board

// Use UART2 alternative 
// Hold down RST button and release when IDE 
// start "uploading..."


void setup() {
  pinMode(7, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(7, HIGH);
  delay(1000);                       // wait for a second
  digitalWrite(7, LOW);
  delay(1000);                       // wait for a second
}
