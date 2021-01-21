

#define GPRS_boot  12

#define relay      13

#define led1       30
#define led2       31
#define led3       32

#define buzzer     33

#define water_flow 7

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
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);

  pinMode(GPRS_boot, OUTPUT);
  digitalWrite(GPRS_boot, LOW);

}



void loop() {
  test();

}


void test() {
  Serial.println("begin..");
  test_ON();
  test_OFF();
  test_ON();
  test_OFF();
  test_ON();
  test_OFF();
}


void test_ON() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(buzzer, HIGH);
  digitalWrite(relay, HIGH);

  delay(1000);
}

void test_OFF() {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(buzzer, LOW);
  digitalWrite(relay, LOW);

  delay(1000);
}
