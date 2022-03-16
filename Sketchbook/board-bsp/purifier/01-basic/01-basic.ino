
#define led30      30 // HIGH -> ON
#define led31      31 // HIGH -> ON
#define buzzer     33 // HIGH -> OFF

#define GPRS_boot  12 // HIGH -> ON
#define relay      10 // HIGH -> ON // relay set jumper to HIGH
#define tap_feedback 6 // default LOW

#define freeGpio1 25
#define freeGpio2 26
#define freeGpio3 27
#define freeGpio4 28

// sensors
#define flow_measure 7 // sensor flow input pulse

#define water_tds  A0 // TDS analog input 
#define water_temp A1 // temperature analog input
#define flood      A2 // top-right 2P XH2.54


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(9600);  // TDS sensor

  pinMode(tap_feedback, INPUT);
  pinMode(flow_measure, INPUT);

  pinMode(led30, OUTPUT);
  pinMode(led31, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);

  digitalWrite(buzzer, HIGH); // buzzer off
  digitalWrite(relay, LOW); // relay ?
  digitalWrite(led31, HIGH); // led31 ON

  pinMode(freeGpio1, OUTPUT);
  pinMode(freeGpio2, OUTPUT);
  pinMode(freeGpio3, OUTPUT);
  pinMode(freeGpio4, OUTPUT);

  delay(1000);
}

void loop() {
  GPIOLoopTest();
  //relaysTest();
  
}


void relaysTest() {
  digitalWrite(relay, HIGH);
  
  int R = digitalRead(6);

  if (R == LOW) {
    digitalWrite(buzzer, LOW); // turn ON?
    digitalWrite(led30, HIGH); // turn ON?
  }
  else {
    digitalWrite(buzzer, HIGH); // turn ON?
    digitalWrite(led30, LOW); // turn ON?
  }
}


void GPIOLoopTest() {
  // put your main code here, to run repeatedly:
  digitalWrite(led30, HIGH);
  digitalWrite(led31, HIGH);
  digitalWrite(buzzer, HIGH);

  //digitalWrite(freeGpio1, HIGH);
  //digitalWrite(freeGpio2, HIGH);
  //digitalWrite(freeGpio3, HIGH);
  //digitalWrite(freeGpio4, HIGH);
  delay(1000);

  digitalWrite(led30, LOW);
  digitalWrite(led31, LOW);
  digitalWrite(buzzer, LOW);

  //digitalWrite(freeGpio1, LOW);
  //digitalWrite(freeGpio2, LOW);
  //digitalWrite(freeGpio3, LOW);
  //digitalWrite(freeGpio4, LOW);
  delay(1000);

}
