
#define tap_feedback 6 // default LOW

#define GPRS_boot  12 // HIGH -> ON

#define relay      10 // HIGH -> ON

#define led1       30 // HIGH -> ON
#define led2       31 // HIGH -> ON

#define buzzer     33 // HIGH -> OFF

#define water_flow 7

#define water_tds  A0 // traditional 
#define water_temp A1 // 
#define flood      A2 // top-right 2P XH2.54


// Serial  = uart0 = USB-TTL
// Serial1 = uart1 = SIM800
// Serial2 = uart2 = TDS_new


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  pinMode(tap_feedback, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  init_off();
  init_on();
}



void init_off() {
  pinMode(GPRS_boot, OUTPUT);
  digitalWrite(GPRS_boot, LOW);
  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  
  digitalWrite(buzzer, HIGH); // buzzer off
  digitalWrite(relay, LOW); // relay off
}

void init_on() {
  pinMode(GPRS_boot, OUTPUT);
  digitalWrite(GPRS_boot, HIGH);
  delay(5000);
  digitalWrite(GPRS_boot, LOW);

  
  //digitalWrite(relay, HIGH); // relay on

}


void loop() {
  //AT_loop();
  read_relay_status();
  //general_test();
  //delay(2000);
}



void AT_loop() {
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }

}

void general_test() {
  digitalWrite(relay, HIGH); // relay on
  digitalWrite(buzzer, HIGH); // relay on
  delay(3000);

  digitalWrite(relay, LOW); // relay on
  digitalWrite(buzzer, LOW); // relay on
  delay(3000);
  }

void read_relay_status() {
  int tap_staus = digitalRead(tap_feedback);

  if (tap_staus == HIGH) {
    digitalWrite (buzzer, HIGH);
    digitalWrite (led1, LOW);
  }
  else {
    digitalWrite(buzzer, LOW);
    digitalWrite(led1, HIGH);
  }
}
