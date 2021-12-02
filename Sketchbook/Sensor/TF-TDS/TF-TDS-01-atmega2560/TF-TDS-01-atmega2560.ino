
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  delay(1000);

  
  Serial.println("Setup Complete!");

}



void loop() {
    while(!Serial2.available()); // wait for the serial port to senr data
    
    int incomingByte = Serial2.read();
    
    Serial.println(incomingByte, HEX);
  }
