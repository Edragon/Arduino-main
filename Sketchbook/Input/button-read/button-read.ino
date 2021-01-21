int dim_ctrl = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(dim_ctrl, INPUT_PULLUP);
  
  //digitalWrite(dim_ctrl, HIGH);

  pinMode(13, OUTPUT);
  
}

void loop() {
  if (digitalRead(dim_ctrl) == HIGH) {
    delayMicroseconds(1000); //此延迟控制电源
    digitalWrite(13, HIGH);
  } else {
    delayMicroseconds(5000);
    digitalWrite(13, LOW);
  }

}
