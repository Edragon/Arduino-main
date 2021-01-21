#include <TinyGsmClient.h>
#include <PubSubClient.h>

#include <SoftwareSerial.h>

#define TINY_GSM_MODEM_SIM800
#define SerialMon Serial


SoftwareSerial SerialAT(2, 3); // RX, TX

#define TINY_GSM_DEBUG SerialMon

// Set phone numbers, if you want to test SMS and Calls
#define SMS_TARGET  "18576608994"
#define CALL_TARGET "18576608994"
bool SMS_test = true;
bool Call_test = true;

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "3gnet";
const char user[] = "";
const char pass[] = "";



TinyGsm modem(SerialAT);
TinyGsmClient gsm(modem);
PubSubClient client(gsm);

const char* broker = "167.88.114.101";

const char* topicRelay1 = "gsm/relay1";
const char* topicRelay2 = "gsm/relay2";

const char* topicDevice = "gsm/device";

const char* topicStatus = "gsm/status";

int relay1 = 6;
int relay2 = 7;

int relayStatus1 = 0;
int relayStatus2 = 0;

//long lastReconnectAttempt = 0;

bool res;
String imei;

//void mqttCallback();
//void GSM_init();
//void GSM_IP();
//void reconnect();
//void GSM_network ();

void setup() {

  SerialMon.begin(9600);
  delay(10);

  SerialAT.begin(9600);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, LOW);



  GSM_init(); // off, boot, restart, connect to service network, register apn, etc
  
  client.setServer(broker, 1883);
  client.setCallback(mqttCallback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  
  client.loop();

}



