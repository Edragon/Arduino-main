
const int IA1 = 16;
const int IA2 = 17;
const int IB1 = 18;
const int IB2 = 19;

int beeper = 5;

void setup() {
  //pinMode(IA1, OUTPUT);
  pinMode(IA2, OUTPUT);
  //pinMode(IB1, OUTPUT);
  pinMode(IB2, OUTPUT);

  ledcAttachPin(IA1, 1);
  ledcAttachPin(IB1, 2);
  ledcAttachPin(beeper, 3);

  // Initialize channels
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(1, 12000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(2, 12000, 8);
  ledcSetup(3, 800, 8);  // channel 3, 500khz, 8bit
}




void loop() {
  //ledcWrite(3, 0);
  digitalWrite(IA2, LOW);
  digitalWrite(IB2, LOW);


  ledcWrite(1, 255); // motor 1 and 2
  ledcWrite(2, 125);

  ledcWrite(3, 0); // buzzer
  delay(5000);


  ledcWrite(1, 0);  // motor 1 and 2
  ledcWrite(2, 0);
  
  ledcWrite(3, 125); // duty 125
  delay(5000);
}
