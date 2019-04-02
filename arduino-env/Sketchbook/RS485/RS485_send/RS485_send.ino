#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  pinMode(8, OUTPUT);//DE/RE Controling pin of RS-485
}

void loop() {
  char getdata='m';

  digitalWrite(8,HIGH);//DE/RE=HIGH Transmit Enabled M1
  Serial.print('9');//Write '9' and Fetch Data From Pro Mini
   
  digitalWrite(8,LOW);//DE/RE=LOW Receive Enabled M1
  delay(1000);
  
  if(mySerial.available()){ //If Serial Data is available
    
    while(mySerial.available() && getdata!='d')
    { 
    getdata=mySerial.read();
    Serial.print(getdata);
    }
    
    Serial.println("");
    }
}
