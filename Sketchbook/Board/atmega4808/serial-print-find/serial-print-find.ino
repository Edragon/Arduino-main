

void setup() {
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.swap(1);
  Serial2.begin(115200);

}


void loop() {
  Serial.println("AT\r\n");
  Serial.println("Test1");
  Serial1.println("Test1");
  
  Serial2.println("Test2");
  delay(100);
}
