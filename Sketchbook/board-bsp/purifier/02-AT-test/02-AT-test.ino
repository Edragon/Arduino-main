


// acurator tap and relay

#define tap_feedback 6 // default LOW
#define relay      10 // HIGH -> ON

// network

#define GPRS_boot  12 // HIGH -> ON

// peripherals

#define led30       30 // HIGH -> ON
#define led31       31 // HIGH -> ON

#define buzzer     33 // HIGH -> OFF

// sensors
#define flow_measure 7 // sensor flow input pulse

#define water_tds  A0 // TDS analog input 
#define water_temp A1 // temperature analog input
#define flood      A2 // top-right 2P XH2.54

// Serial  = uart0 = USB-TTL
// Serial1 = uart1 = SIM800
// Serial2 = uart2 = TDS_new


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  
  pinMode(tap_feedback, INPUT);
  pinMode(led30, OUTPUT);
  pinMode(led31, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  
  pinMode(flow_measure, INPUT);
  
  init_off();
  init_on();
}



void init_off() {
  // turn off leds, buzzers, relay
  pinMode(GPRS_boot, OUTPUT);
  digitalWrite(GPRS_boot, LOW);
  digitalWrite(led30, LOW);
  digitalWrite(led31, LOW);
  digitalWrite(buzzer, HIGH);
  digitalWrite(relay, LOW);
}

void init_on() {
  // turn on SIM800 
  pinMode(GPRS_boot, OUTPUT);
  digitalWrite(GPRS_boot, HIGH);
  delay(5000);
  digitalWrite(GPRS_boot, LOW);
}



void loop() {
  AT_loop();
  //read_relay_status();

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




void read_relay_status() {
  int tap_staus = digitalRead(tap_feedback);

  if (tap_staus == HIGH) {
    digitalWrite (led30, HIGH);
  }
  else {
    digitalWrite(led30, LOW);
  }
}
