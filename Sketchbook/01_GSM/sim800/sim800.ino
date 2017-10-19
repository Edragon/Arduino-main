

#include <SoftwareSerial.h>

#define SIM800_TX_PIN 2
#define SIM800_RX_PIN 3

//Create software serial object to communicate with SIM800
SoftwareSerial serialSIM800(SIM800_TX_PIN, SIM800_RX_PIN);

void relayTest() {
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  delay(1000);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  delay(1000);
}

void GsmBoot() {
  digitalWrite(5, HIGH);
  //delay(5000);
  //digitalWrite(5, LOW);
}

void setup() {
  //Begin serial comunication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  while (!Serial);

  //Being serial communication witj Arduino and SIM800
  serialSIM800.begin(9600);
  delay(1000);

  Serial.println("Setup Complete!");

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  GsmBoot();
}


void loop() {
  //relayTest();
  
  //Read SIM800 output (if available) and print it in Arduino IDE Serial Monitor
  if (serialSIM800.available()) {
    Serial.write(serialSIM800.read());
  }
  //Read Arduino IDE Serial Monitor inputs (if available) and send them to SIM800
  if (Serial.available()) {
    serialSIM800.write(Serial.read());
  }
}
