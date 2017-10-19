#include <FS.h>  
#include <WiFiManager.h> 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println("format...");
  SPIFFS.format();
  Serial.println();
  Serial.println("done...");
  WiFiManager wifiManager;
  wifiManager.resetSettings();
}

void loop() {
  // put your main code here, to run repeatedly:

}
