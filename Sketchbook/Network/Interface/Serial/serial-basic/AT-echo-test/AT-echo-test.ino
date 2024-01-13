#define GPRS_boot  12
void setup() {

  Serial.begin(9600);
  Serial1.begin(9600);
  
  pinMode(GPRS_boot, OUTPUT);
  digitalWrite(GPRS_boot, LOW);
  delay(3000);
  digitalWrite(GPRS_boot, HIGH);
  delay(3000);
  digitalWrite(GPRS_boot, LOW);
  
  Serial.println("Boot!");
  
}


void loop() {

  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }
}
