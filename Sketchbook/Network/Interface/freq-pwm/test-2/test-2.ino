int pulse_counter = 0;

void interrupt_handler()
{
  pulse_counter++;
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
}


void setup()
{
  Serial.begin(115200);
  // For noise suppression, enable pullup on interrupt pin
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  digitalWrite(2, HIGH);
  attachInterrupt(0, interrupt_handler, RISING);
}


void loop() {
  // Keep LCD blank till a pulse comes in
  if (pulse_counter > 0)
  {

    Serial.println(pulse_counter);
    delay(100);

  }
}
