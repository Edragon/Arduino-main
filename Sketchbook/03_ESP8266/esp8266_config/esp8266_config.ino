#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <FS.h>



void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("....");
  delay(1000);
  Serial.println("....");
  //Serial.println("Start to clean....");
  //SPIFFS.format();
  //Serial.println("FS done....");
}

void loop() {
  int id = ESP.getChipId();
  String id_str = String(id);
  char char_id[11];
  id_str.toCharArray(char_id, 11);

  // error
  //char char_id[11];
  //dtostrf(id, 11, 0 char_id);  //double to string
  //char char_id = (char)id; // only for 0-127
  Serial.print("int number is: ");
  Serial.println(id);  // int ID
  
  Serial.print("result: ");
  Serial.println(char_id);
  // put your main code here, to run repeatedly:
  //Serial.printf(" ESP8266 Chip id = %06X\n", ESP.getChipId());
  delay(1000);
}
