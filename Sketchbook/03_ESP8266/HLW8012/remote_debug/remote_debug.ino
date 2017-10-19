
#include "power.h"
//#include <FS.h>
#include "RemoteDebug.h"        //https://github.com/JoaoLopesF/RemoteDebug
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

//WiFiServer server(80);

RemoteDebug Debug;
ESP8266PowerClass power_dev;

unsigned long previousMillis = 0;
unsigned long mTimeSeconds = 0;
const long interval = 2000;

/*
    this->power_param = {12.286,0};
    this->current_param = {14.818,0};
    this->voltage_param = {0.435,0};
    
    this->power_pin = 14;   // CF
    this->current_voltage_pin = 12;  //CF1
    this->sel_pin = 13;   // Chip select
*/

const char* ssid = "sscz_48FD";
const char* password = "electrodragon";

// Host mDNS
#define HOST_NAME "8012"

double power;
double current;
double cfrequency;
double voltage;

char PS[6];
char CS[6];
char CFS[6];
char VS[6];

void setup() {
  Serial.begin(115200);
  //SPIFFS.format();
  //Serial.println("mounting FS...");
  //Serial.begin(74880);
  //Serial1.begin(115200);
  //pinMode(16, OUTPUT);
  //digitalWrite(16, HIGH);


  //power_dev.setPowerParam(12.65801022, 0.0);
  //power_dev.setVoltageParam(0.45039823, 0.0);
  //power_dev.setCurrentParam(19.52, -85.9);

  power_dev.enableMeasurePower();
  power_dev.selectMeasureCurrentOrVoltage(CURRENT);
  power_dev.startMeasure(); // mills

  // WiFi connection

  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Register host name in WiFi and mDNS

  //String hostNameWifi = HOST_NAME;
  //hostNameWifi.concat(".local");

  WiFi.hostname(HOST_NAME);

  if (MDNS.begin(HOST_NAME)) {
    Serial.print("* MDNS responder started. Hostname -> ");
    Serial.println(HOST_NAME);
  }
  //server.begin();
  //Serial.println("TCP server started");

  // Add service to MDNS-SD
  //MDNS.addService("http", "tcp", 80);
  MDNS.addService("telnet", "tcp", 23);

  // Initialize the telnet server of RemoteDebug

  Debug.begin(HOST_NAME); // Initiaze the telnet server
  Debug.setSerialEnabled(true);
  Debug.setResetCmdEnabled(true); // Enable the reset command

}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    mTimeSeconds++;

    Debug.printf("* Times (2seconds): %u seconds (VERBOSE)\n", mTimeSeconds);

    power = power_dev.getPower();
    current = power_dev.getCurrent();
    voltage = power_dev.getVoltage();

    cfrequency = power_dev.getCurrFrequency();
    Serial.print("power: ");
    Serial.println(power);
    Serial.print("current: ");
    Serial.println(current);
    Serial.print("voltage: ");
    Serial.println(voltage);
    Serial.print("current frequency: ");
    Serial.println(cfrequency);
    
    dtostrf(power, 4, 2, PS);
    dtostrf(current, 4, 2, CS);
    dtostrf(voltage, 4, 2, VS);
    dtostrf(cfrequency, 4, 2, CFS);
    
    Debug.printf("* Power: %s ", PS);
    Debug.printf("* Current: %s ", CS);
    Debug.printf("* Voltage: %s ", VS);
    Debug.printf("* Current FREQ: %s,\n", CFS);
  }
  // Remote debug over telnet

  Debug.handle();

  // Give a time for ESP8266

  yield();
}


