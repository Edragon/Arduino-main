
#define TINY_GSM_MODEM_SIM800
#define SerialMon Serial

#include <SoftwareSerial.h>
SoftwareSerial SerialAT(2, 3); // RX, TX

#define TINY_GSM_DEBUG SerialMon

// Set phone numbers, if you want to test SMS and Calls
#define SMS_TARGET  "18576608994"
//#define CALL_TARGET "+380xxxxxxxxx"

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "3gnet";
const char user[] = "";
const char pass[] = "";

#include <TinyGsmClient.h>

TinyGsm modem(SerialAT);

void pwr() {
  Serial.println("booting modem ..");
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  delay(5000);
  digitalWrite(5, HIGH);
  delay(2000);
}

void setup() {
  SerialMon.begin(9600);
  delay(10);

  SerialAT.begin(9600);


}

void loop() {
  pwr();
  delay(5000);

  modem.init(); // factory reset AT&FZ, echo off "ATE0"

  for (int i = 0; i <= 5; i++) {
    Serial.println(".................");

    String modemInfo = modem.getModemInfo(); // command AT+I?
    DBG("Modem:", modemInfo);
    delay(1000);

    String ccid = modem.getSimCCID(); // AT+ICCID?
    DBG("CCID:", ccid);
    delay(1000);

    String imei = modem.getIMEI();  // AT+GSN?
    DBG("IMEI:", imei);
    delay(1000);

    //turn on this only when network avaialble
    //String cop = modem.getOperator();
    //DBG("Operator:", cop);
    //delay(1000);

    IPAddress local = modem.localIP();
    DBG("Local IP:", local);
    delay(1000);

    int csq = modem.getSignalQuality();   // AT+CSQ?
    DBG("Signal quality:", csq);
    delay(1000);

    int battLevel = modem.getBattPercent();
    DBG("Battery lavel:", battLevel);
    delay(1000);

    float battVoltage = modem.getBattVoltage() / 1000.0F;
    DBG("Battery voltage:", battVoltage);
    delay(1000);

  }

  if (modem.poweroff()) {
    Serial.println("power off now..");
  }

}

