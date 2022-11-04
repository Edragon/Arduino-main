#include <SoftwareSerial.h>


#define SIM800_TX_PIN 4
#define SIM800_RX_PIN 5

//#define SIM800_TX_PIN 2
//#define SIM800_RX_PIN 3

#define relay_control 6
#define GSM_boot 8

//Create software serial object to communicate with SIM800
SoftwareSerial serialSIM800(SIM800_TX_PIN, SIM800_RX_PIN);



void setup() {
  //Begin serial comunication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(19200);
  while (!Serial);

  //Being serial communication witj Arduino and SIM800
  serialSIM800.begin(19200);
  delay(1000);

  pinMode(GSM_boot, OUTPUT);
  digitalWrite(GSM_boot, LOW); // keep low default

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




bool GSM_statuscheck() {
  serialSIM800.println("AT"); // try command to turn off GSM module by AT commands
  delay(100);
  if ( serialSIM800.find("OK") ) {
    return true;
  }
  else {
    return false;
  }

}


void relayTest() {
  digitalWrite(relay_control, HIGH);
  delay(1000);
  digitalWrite(relay_control, LOW);
  delay(1000);
}

void GsmReboot() {
  if ( GSM_statuscheck() == true ) {
    Serial.println("GSM active, skip boot!");    
  }
  if ( GSM_statuscheck() == false ) {
    Serial.println("GSM NOT active, will boot now!");
    //boot GSM module
    digitalWrite(GSM_boot, HIGH);
    delay(5000);
    digitalWrite(GSM_boot, LOW);
    
  }
  // serialSIM800.println("AT+CPOWD=1");
}
