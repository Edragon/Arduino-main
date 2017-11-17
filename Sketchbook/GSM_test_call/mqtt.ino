

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    
    GSM_network();
    
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("gsm")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(topicDevice, "gsm online");
      // ... and resubscribe

      client.subscribe(topicDevice);
      client.subscribe(topicRelay1);
      client.subscribe(topicRelay2);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);

    }
  }
}


void mqttCallback(char* topic, byte* payload, unsigned int len) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.write(payload, len);
  Serial.println();


  if (String(topic) == topicRelay1) {

    if ((char)payload[0] == '1') {
      digitalWrite(relay1, HIGH);
      relayStatus1 = 1;
      DBG("relay 1 HIGH");
    } else {
      digitalWrite(relay1, LOW);
      relayStatus1 = 0;
      DBG("relay 1 LOW");
    }
    client.publish(topicStatus, relayStatus1 ? "r1 ON" : "r1 OFF");
  }

  if (String(topic) == topicRelay2) {

    if ((char)payload[0] == '1') {
      digitalWrite(relay2, HIGH);
      relayStatus2 = 1;
      DBG("relay 2 HIGH");
    } else {
      digitalWrite(relay2, LOW);
      relayStatus2 = 0;
      DBG("relay 2 LOW");
    }
    client.publish(topicStatus, relayStatus2 ? "r2 ON" : "r2 OFF");
  }

}




/*

  boolean mqttConnect() {
  Serial.print("Connecting to ");
  Serial.print(broker);
  if (!client.connect("gsm")) {
    Serial.println(" fail");
    return false;
  }
  Serial.println(" OK");
  client.publish(topicDevice, "gsm started");
  client.subscribe(topicLed);
  return client.connected();
  }


  void mqtt_loop () {
  if (client.connected()) {
    client.loop();
  } else {
    DBG("retrying ...");
    // Reconnect every 10 seconds
    unsigned long t = millis();
    if (t - lastReconnectAttempt > 10000L) {
      lastReconnectAttempt = t;
      if (mqttConnect()) {
        lastReconnectAttempt = 0;
      }
    }
  }
  }
*/
