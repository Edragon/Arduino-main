
#define GSM_Boot 12
#define relay 13

void setup() {

  pinMode(GSM_Boot, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);

  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("ready.");



  digitalWrite(GSM_Boot, HIGH);
  delay(5000);
  digitalWrite(GSM_Boot, LOW);

}


void loop() {
  
  Serial1.println("AT+IPR=0");
  
  if (Serial1.find("OK")) {
    Serial.println("find OK");
  }

  
//  if (Serial1.available()) {
//    //delay(5);
//    Serial.write(Serial1.read());
//  }


}
