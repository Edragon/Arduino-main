#include <SoftwareSerial.h>
#define SSID        "sscz_48FD"
#define PASS        "electrodragon"
#define DST_IP      "220.181.111.85"    //baidu.com
SoftwareSerial dbgSerial(3, 4); // RX, TX

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.setTimeout(5000);
  dbgSerial.begin(115200);  //can't be faster than 19200 for softserial
  Serial.println("ESP8266 Demo");
  ESPinit();

}

void loop()
{
  sendTCP();
}


