void GSM_gprsOFF() {
  modem.gprsDisconnect();
  DBG("GPRS disconnected");
}

void pwr() {
  modem.poweroff();
  delay (1000);
  Serial.println("booting modem ..");
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  delay(5000);
  digitalWrite(5, HIGH);
  delay(2000);
}

void GSM_init() {
  pwr();
  //delay(2000);

  //modem.init(); // factory reset AT&FZ, echo off "ATE0"

  //init and network
  Serial.println("Initializing modem...");
  modem.restart();


  DBG("Waiting for network...");
  if (!modem.waitForNetwork()) {
    Serial.println(" fail");
    while (true);
  }
  Serial.println(" OK");

  GSM_network ();

}

void GSM_network() {
  DBG("Connect to APN...");
  if (!modem.gprsConnect(apn, user, pass)) {
    Serial.println(" fail");
    while (true);
  }
  Serial.println(" OK");
  
  delay(5000);
  GSM_IP();
}



void GSM_IP () {
  IPAddress local = modem.localIP();
  DBG("Local IP:", local);
  delay(1000);
}

void GSM_basic() {
  for (int i = 0; i < 2; i++) {
    Serial.println(".................");

    String modemInfo = modem.getModemInfo(); // command AT+I?
    DBG("Modem:", modemInfo);
    delay(1000);

    String ccid = modem.getSimCCID(); // AT+ICCID?
    DBG("CCID:", ccid);
    delay(1000);

    imei = modem.getIMEI();  // AT+GSN?
    DBG("IMEI:", imei);
    delay(1000);

    String cop = modem.getOperator();
    DBG("Operator:", cop);
    delay(1000);

    GSM_IP ();

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
#if defined(SMS_TARGET)
  if (SMS_test == true) {
    res = modem.sendSMS(SMS_TARGET, String("Hello from ") + imei);
    DBG("SMS:", res ? "OK" : "fail");
    delay(2000);
    SMS_test = false;
  } else {
    DBG("already sent the message ..");
  }
#endif


#if defined(CALL_TARGET)
  if (Call_test == true) {
    DBG("Calling:", CALL_TARGET);

    // This is NOT supported on M590
    res = modem.callNumber(CALL_TARGET);
    DBG("Call:", res ? "OK" : "fail");

    if (res) {
      delay(5000L);

      res = modem.callHangup();
      DBG("Hang up:", res ? "OK" : "fail");
    }
    Call_test = false;
  } else {
    DBG("already called ...");
  }

#endif


}


