boolean mqttConnect() {
  Serial.print("Connecting to MQTT: ");
  Serial.print(broker);
  
  if (!mqtt.connect("GsmClientTest")) {
    Serial.println(" fail");
    return false;
  }
  
  Serial.println(" OK");
  mqtt.publish(topicDevice, "GSM Relay Board Online Now");

  //mqtt.publish(topicDevice, imei); // ?

  mqtt.subscribe(topicDevice);
  mqtt.subscribe(topicRelay1);
  mqtt.subscribe(topicRelay2);

  Serial.print("MQTT status: ") ;
  Serial.println(mqtt.connected()) ;
  
  return mqtt.connected();
  
  
}


//void MQTT_reconnect() {
//
//
//  // Loop until we're reconnected
//  while (!mqtt.connected()) {
//
//    //GSM_network();
//
//    Serial.print("Attempting MQTT connection...");
//    Serial.print(broker);
//    // Attempt to connect
//    if (mqtt.connect("gsm")) {
//      Serial.println("... connected");
//      
//      Serial.print("MQTT rc=");
//      Serial.println(mqtt.state());
//      
//      // Once connected, publish an announcement...
//      mqtt.publish(topicDevice, "gsm online");
//      // ... and resubscribe
//
//      mqtt.subscribe(topicDevice);
//      mqtt.subscribe(topicRelay1);
//      mqtt.subscribe(topicRelay2);
//
//    } else {
//      Serial.print("failed, rc=");
//      Serial.print(mqtt.state());
//      Serial.println(" try again in 5 seconds");
//      // Wait 5 seconds before retrying
//      delay(5000);
//
//    }
//  }
//}


void mqttCallback(char* topic, byte* payload, unsigned int len) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.write(payload, len);
  Serial.println();


  // topic gsm/relay1 or 2, receive 1 or 0 to switch on or off relay
  if (String(topic) == topicRelay1) {

    if ((char)payload[0] == '1') {
      digitalWrite(relay1, HIGH);
      relayStatus1 = 1;
      Serial.println("relay 1 HIGH");
      mqtt.publish(topicDevice, "GSM Relay Board Relay1 activated");
    } else {
      digitalWrite(relay1, LOW);
      relayStatus1 = 0;
      Serial.println("relay 1 LOW");
    }
    mqtt.publish(topicStatus, relayStatus1 ? "r1 ON" : "r1 OFF");
  }


  if (String(topic) == topicRelay2) {

    if ((char)payload[0] == '1') {
      digitalWrite(relay2, HIGH);
      relayStatus2 = 1;
      Serial.println("relay 2 HIGH");
      mqtt.publish(topicDevice, "GSM Relay Board Relay2 activated");
    } else {
      digitalWrite(relay2, LOW);
      relayStatus2 = 0;
      Serial.println("relay 2 LOW");
    }
    mqtt.publish(topicStatus, relayStatus2 ? "r2 ON" : "r2 OFF");
  }

}
