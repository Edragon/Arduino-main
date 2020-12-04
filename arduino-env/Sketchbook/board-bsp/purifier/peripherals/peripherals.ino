#define GPRS_boot 12;

#define relay 13;

#define led1 30;
#define led2 31;
#define led3 32;

#define buzzer 33;

#define water_flow 7;

#define water_tds A0; // traditional 

#define water_temp A1;

// Serial  = uart0 = USB-TTL
// Serial1 = uart1 = SIM800
// Serial2 = uart2 = TDS_new


void setup() {

  pinMode(led1, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  
  digitalWrite(led1, HIGH);   
  digitalWrite(buzzer, HIGH);  
  digitalWrite(relay, HIGH);  
   
}


void loop() {

}
