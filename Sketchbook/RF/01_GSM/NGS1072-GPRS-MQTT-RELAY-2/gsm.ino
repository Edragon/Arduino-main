
void pwr() {
  Serial.println("booting modem ..");

  modem.poweroff();
  digitalWrite(modem_boot, LOW);
  delay (1000);
  digitalWrite(modem_boot, HIGH);
  delay(5000);
  digitalWrite(modem_boot, LOW);
  delay(2000);
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  Serial.println(modemInfo);
}


// power up and wait until network connected
void GSM_init() {

  //delay(2000);

  //modem.init(); // factory reset AT&FZ, echo off "ATE0"

  //init and network
  //Serial.println("Initializing modem...");
  pwr();
  //modem.restart();
  GSM_network ();
}


// connect to network
void GSM_network() {
  Serial.print("Waiting for network...");
  if (!modem.waitForNetwork()) {
    Serial.println(" fail");
    while (true);
  }
  Serial.println(" OK");

  Serial.print("Connecting to ");
  Serial.print(apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    Serial.println(" fail");
    while (true);
  }
  Serial.println(" OK");
  client.setServer(broker, 1883);
  client.setCallback(mqttCallback);
}
