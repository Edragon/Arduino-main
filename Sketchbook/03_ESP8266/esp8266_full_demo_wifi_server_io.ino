/*
 *  EasyloT sketch tweeked by Barnabybear - original @
 *  http://iot-playground.com/2-uncategorised/40-esp8266-wifi-relay-switch-arduino-ide
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */


#include <ESP8266WiFi.h>


const char* ssid = "14L";
const char* password = "19940116";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
 /* pinMode(3, OUTPUT);
  digitalWrite(3, 0);
  /*pinMode(4, OUTPUT);
  digitalWrite(4, 0);
  pinMode(5, OUTPUT);
  digitalWrite(5, 0);
  pinMode(6, OUTPUT);
  digitalWrite(6, 0);
  pinMode(7, OUTPUT);
  digitalWrite(7, 0);
  pinMode(8, OUTPUT);
  digitalWrite(8, 0);
  pinMode(9, OUTPUT);
  digitalWrite(9, 0);
  pinMode(10, OUTPUT);
  digitalWrite(10, 0);
  pinMode(11, OUTPUT);
  digitalWrite(11, 0);
  pinMode(12, OUTPUT);
  digitalWrite(2, 0);
  pinMode(13, OUTPUT);
  digitalWrite(13, 0);
  pinMode(14, OUTPUT);
  digitalWrite(14, 0);
  pinMode(15, OUTPUT);
  digitalWrite(15, 0);
  pinMode(16, OUTPUT);
  digitalWrite(16, 0);*/
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started @ ");

  // Print the IP address & instructions
  Serial.println(WiFi.localIP());
  Serial.println("To control GPIO, open your web browser.");
  Serial.println("To set GPIO 0 high, type:");
  Serial.print(WiFi.localIP());
  Serial.println("/gpio/1");
  Serial.println("To set GPIO 0 low, type:");
  Serial.print(WiFi.localIP());
  Serial.println("/gpio/0");
  Serial.println("To toggle GPIO 0, type:");
  Serial.print(WiFi.localIP());
  Serial.println("/gpio/0");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    delay(100);
    return;
  }
  
  // Commented out by BB as gets stuck in while loop.
  // Wait until the client sends some data
  //Serial.println("new client");
  //while(!client.available()){
  //  delay(1);
  //}
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  if (req.indexOf("/gpio/0") != -1)
    val = 0;
  else if (req.indexOf("/gpio/1") != -1)
    val = 1;
  else if (req.indexOf("/gpio/4") != -1)
   val = (!digitalRead(0)); // <<<<< Edit: insert /gpio/3 lines after this line.
  else {
    Serial.println("invalid request");
    client.print("HTTP/1.1 404\r\n");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(2, val);
 /*digitalWrite(3, val);
  /* digitalWrite(4, val);
  digitalWrite(5, val);
  digitalWrite(6, val);
   digitalWrite(7, val);
  digitalWrite(8, val);
  digitalWrite(9, val);
  digitalWrite(10, val);
  digitalWrite(11, val);
  digitalWrite(12, val);
  digitalWrite(13, val);
  digitalWrite(14, val);
  digitalWrite(15, val);
  digitalWrite(16, val);*/
  
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
} 
