

#define GPRS_boot  12

#define MCU_relay      13

#define led1       30
#define led2       31
#define led3       32

#define buzzer     33

#define water_flow 7

#define tap_input 6

#define water_tds  A0 // traditional 
#define water_temp A1
#define flood      A2

// Serial  = uart0 = USB-TTL
// Serial1 = uart1 = SIM800
// Serial2 = uart2 = TDS_new


void setup() {
  Serial.begin(115200);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  pinMode(tap_input, OUTPUT);
  pinMode(tap_input, INPUT);
  //digitalWrite(tap_input, HIGH);

  pinMode(MCU_relay, OUTPUT);

  // turn off GPRS
  pinMode(GPRS_boot, OUTPUT);
  digitalWrite(GPRS_boot, LOW);

  // turn off buzzer
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
}


void loop() {
  check_pin6();
}


void check_pin6() {
  int relay_stat = digitalRead(tap_input);

  if (relay_stat == HIGH) {
    Serial.println("status low");
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);

    digitalWrite(MCU_relay, HIGH);
    digitalWrite(buzzer, LOW);

  } else {
    Serial.println("status high");
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);

    digitalWrite(MCU_relay, LOW);
    digitalWrite(buzzer, HIGH);
  }

}
