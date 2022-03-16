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
  Serial2.begin(9600);  // TDS sensor
  Serial3.begin(115200);  // TDS sensor

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
  digitalWrite(led30, LOW);    // off
  digitalWrite(led31, LOW);    // off
  digitalWrite(buzzer, HIGH); // off
  digitalWrite(relay, LOW);   // off?
  delay(1000);

  PeripheralsTest();

}

void init_on() {
  // turn on SIM800
  pinMode(GPRS_boot, OUTPUT);
  digitalWrite(GPRS_boot, HIGH);
  delay(5000);
  digitalWrite(GPRS_boot, LOW);

  testCMD();

}



void loop() {


  AT_loop();
  //AT_loop3();
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

void AT_loop3() {
  if (Serial3.available()) {
    Serial.write(Serial3.read());
  }
  if (Serial.available()) {
    Serial3.write(Serial.read());
  }
}

void testCMD()
{

  Serial1.println("AT\r\n");
  delay(20);
  if (Serial1.find("OK")) {
    Serial.println("Feedback OK");
  }
  delay(500);

  Serial1.println("AT+CPIN?\r\n");
  delay(20);
  if (Serial1.find("OK")) {
    Serial.println("CPIN OK2");
  }
  delay(500);

  Serial1.println("AT+BTPOWER=1\r\n");
  delay(500);
  if (Serial1.find("OK")) {
    Serial.println("BT OK3");
  }
  delay(2000);
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

void PeripheralsTest() {
  digitalWrite(led30, HIGH);    // off
  digitalWrite(led31, HIGH);    // off
  digitalWrite(buzzer, LOW); // off
  digitalWrite(relay, HIGH);   // off?
  delay(5000);

  digitalWrite(led30, LOW);    // off
  digitalWrite(led31, LOW);    // off
  digitalWrite(buzzer, HIGH); // off
  digitalWrite(relay, LOW);   // off?
  delay(1000);
}
