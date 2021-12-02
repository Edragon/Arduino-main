
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
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(buzzer, HIGH);
  digitalWrite(relay, LOW);
}

void init_on() {
  pinMode(GPRS_boot, OUTPUT);
  digitalWrite(GPRS_boot, HIGH);
  delay(5000);
  digitalWrite(GPRS_boot, LOW);
}


void AT_loop() {
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }
  
}

void read_relay_status() {
  int tap_staus = digitalRead(tap_feedback);

  if (tap_staus == HIGH) {
    digitalWrite (led1, HIGH);
  }
  else {
    digitalWrite(led1, LOW);
  }
}

void loop() {
  AT_loop();
  read_relay_status();
  
  //delay(2000);
}
