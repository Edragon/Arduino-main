
#include <FS.h>                   //this needs to be first, or it all crashes and burns...

void setup() {
  //ESP.wdtDisable();
  //ESP.wdtEnable(WDTO_8S);

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
  //ESP.wdtFeed();
  // put your main code here, to run repeatedly:

}
