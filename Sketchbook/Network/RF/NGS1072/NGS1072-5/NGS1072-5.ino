#define TINY_GSM_MODEM_SIM800

#define gsm_tx  4
#define gsm_rx  5

#define relay_1  6
#define relay_2  7
#define gms_pwr 8
#define led 13

#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

// #define SerialMon Serial // this DBG is not working ..
//#define TINY_GSM_DEBUG SerialMon

SoftwareSerial SerialAT(gsm_rx, gsm_tx); // RX, TX
TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

#define modem_boot 8

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "3gnet";
const char user[] = "";
const char pass[] = "";


const char* broker = "167.88.114.101";  // iot.electrodragon.com

const char* topicRelay1 = "gsm/relay1";
const char* topicRelay2 = "gsm/relay2"; // relay control input

const char* topicDevice = "gsm/device"; // device online feedback

const char* topicStatus = "gsm/status"; // relay status feedback

int relay1 = 6;
int relay2 = 7;

int relayStatus1 = 0;
int relayStatus2 = 0;

long lastReconnectAttempt = 0;

//bool res;
String imei;
String modemInfo;

// moved to gsm.ino and mqtt.ino
//void mqttCallback();
//void MQTT_reconnect();

//void GSM_init();
//void GSM_IP();
//void GSM_network ();

void setup() {


  Serial.begin(9600); // badrate 9600 is stable for softserial
  delay(10);

  SerialAT.begin(9600);
  delay(1000);


  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  pinMode(modem_boot, OUTPUT);
  
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  delay(3000);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);

  // modem setup
  GSM_init(); // off, boot, restart, connect to service network, register apn, etc


}

//void loop() {
//  if (!mqtt.connected()) {
//    MQTT_reconnect();
//  }
//  mqtt.loop();
//
//}

void loop() {

  if (mqtt.connected() ) {
    mqtt.loop();
    
  } else {
    Serial.println("Connection lost ..");
    // Reconnect every 10 seconds
    unsigned long t = millis();
    if (t - lastReconnectAttempt > 10000L) {
      lastReconnectAttempt = t;
      if (mqttConnect()) {
        lastReconnectAttempt = 0;
      }
    }
  }

}
