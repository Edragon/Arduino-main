
#include <FS.h>                   //this needs to be first, or it all crashes and burns...

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.println("....");
  delay(1000);
  Serial.println("....");
  delay(1000);
  Serial.println("....");
  Serial.println("Start to clean....");
  SPIFFS.format();
  Serial.begin(115200);
  Serial.println("FS done....");

}

void loop() {
  // put your main code here, to run repeatedly:

}
