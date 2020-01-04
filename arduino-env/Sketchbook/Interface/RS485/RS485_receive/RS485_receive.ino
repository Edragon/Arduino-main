void setup() {
    
  //Serial1.begin(9600);//Uncomment for Arduino Lenardo
  Serial.begin(9600);
  //while(!Serial1);//Uncomment for Arduino Lenardo

  pinMode(13, OUTPUT);//Led Connected
  pinMode(8, OUTPUT);//DE/RE Controling pin of RS-485
}

void loop() {
  // put your main code here, to run repeatedly:
  char getdata='c';

  digitalWrite(13,LOW);//Led OFF
  digitalWrite(8,LOW);//DE/RE=LOW Receive Enabled

   if(Serial.available()){
    getdata=Serial.read();
    }
    
   if(getdata=='9'){
      digitalWrite(8,HIGH);//DE/RE=HIGH Transmit Enabled 
      Serial.print("AcruxTek");
      Serial.print("Isld");
    }
    
delay(2000);
digitalWrite(13,HIGH);//Led ON
delay(2000);
}
