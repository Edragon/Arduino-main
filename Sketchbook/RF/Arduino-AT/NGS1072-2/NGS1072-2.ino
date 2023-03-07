#include <SoftwareSerial.h>


#define SIM800_TX_PIN 4
#define SIM800_RX_PIN 5
#define relay_control 6
#define GSM_boot 8

//Create software serial object to communicate with SIM800
SoftwareSerial serialSIM800(SIM800_TX_PIN, SIM800_RX_PIN);

void relayTest() {
  digitalWrite(relay_control, HIGH);
  delay(1000);
  digitalWrite(relay_control, LOW);
  delay(1000);
}

void GsmBoot() {
  digitalWrite(GSM_boot, HIGH);
}

void GsmReboot() {
  serialSIM800.println("AT+CPOWD=1"); // try command to turn off GSM module by AT commands
  digitalWrite(GSM_boot, LOW); // release high status
  delay(1000);
  if ( serialSIM800.find("NORMAL") ) {
    Serial.println("GSM is already active, now shutdown and reboot! ");
  }
  digitalWrite(GSM_boot, HIGH); // restart
  delay(5000);
  digitalWrite(GSM_boot, LOW); // 
  delay(3000);
  serialSIM800.println("AT");
  if ( serialSIM800.find("OK") ) {
    Serial.println("GSM working!");
    }
}

void setup() {
  //Begin serial comunication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  while (!Serial);

  //Being serial communication witj Arduino and SIM800
  serialSIM800.begin(9600);
  delay(1000);
  
  pinMode(GSM_boot, OUTPUT);
  
  //relayTest();
  GsmReboot();
  
  Serial.println("Setup Complete, try to send AT commands on serial monitor!");
}


void loop() {
  
  //Read SIM800 output (if available) and print it in Arduino IDE Serial Monitor
  if (serialSIM800.available()) {
    Serial.write(serialSIM800.read());
  }
  //Read Arduino IDE Serial Monitor inputs (if available) and send them to SIM800
  if (Serial.available()) {
    serialSIM800.write(Serial.read());
  }
}
