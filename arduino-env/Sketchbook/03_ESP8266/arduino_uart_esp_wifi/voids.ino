
void ESPinit() {
  //test if the module is ready
  dbgSerial.println("AT+RST");
  delay(5000);
  dbgSerial.println("AT");
  delay(1000);
  if (dbgSerial.find("OK"))
  {
    Serial.println("Module is ready");
  }
  else
  {
    Serial.println("Module have no response.");
    while (1);
  }
  delay(1000);

  //connect to the wifi
  boolean connected = false;
  for (int i = 0; i < 5; i++)
  {
    if (connectWiFi())
    {
      connected = true;
      break;
    }
  }
  if (!connected) {
    while (1);
  }
  delay(5000);
  //print the ip addr
  /*Serial.println("AT+CIFSR");
    dbgSerial.println("ip address:");
    while (Serial.available())
    dbgSerial.write(Serial.read());*/
  //set the single connection mode
  dbgSerial.println("AT+CIPMUX=0");
}


void sendTCP() {
  // send get HTTP command to baidu and get feedback
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += DST_IP;
  cmd += "\",80";
  dbgSerial.println(cmd);
  Serial.println(cmd);
  if (dbgSerial.find("Error")) return;
  cmd = "GET / HTTP/1.0\r\n\r\n";
  dbgSerial.print("AT+CIPSEND=");
  dbgSerial.println(cmd.length());
  
  if (dbgSerial.find(">"))
  {
    Serial.print(">");
  } else
  {
    dbgSerial.println("AT+CIPCLOSE");
    Serial.println("connect timeout");
    delay(1000);
    return;
  }
  dbgSerial.print(cmd);
  delay(2000);
  //Serial.find("+IPD");
  while (dbgSerial.available())
  {
    char c = dbgSerial.read();
    Serial.write(c);
    if (c == '\r') Serial.print('\n');
  }
  Serial.println("====");
  delay(1000);
}



boolean connectWiFi()
{
  dbgSerial.println("AT+CWMODE=1");
  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  Serial.println(cmd);
  dbgSerial.println(cmd);
  delay(2000);
  if (dbgSerial.find("OK"))
  {
    Serial.println("OK, Connected to WiFi.");
    return true;
  } else
  {
    Serial.println("Can not connect to the WiFi.");
    return false;
  }
}


