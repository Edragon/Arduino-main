/*
 * This sketch let you control an NodeMCU motorshield with attached motors (wifi car),
 * from a smart phone browser by using the orientation of the smart phone.
 * See at the following link, or other vendors.
 * http://www.smartarduino.com/2wd-wifi-rc-smart-car-with-nodemcu-shield-for-esp-12e_p94572.html
 *
 * Program: wifi-car-AP.ino, 2015-11-19 RR
 *  100 ms cycle time are too short - 200 ms aere O2
 * 2015-11-27 - fix AP mode, Rudolf Reuter
 * 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "FS.h"

#define DBG_OUTPUT_PORT Serial

const short int BUILTIN_LED2 = 16;  //GPIO16
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
// Access Point mode for car use, Station Access mode for software development.
// comment next line for Station Access to WiFi router
#define ACCESSPOINT
#ifdef ACCESSPOINT
const char* ssid = "wifi-car";
const char* password = "";

#else
// Station Access mode, adopt strings to your WiFi router
const char* ssid = ".......";
const char* password = ".......";
#endif

int motorASpeed = 1023;
int motorBSpeed = 1023;
int motorAForward = 1;
int motorBForward = 1;

// Create an instance of the server
// specify the port to listen on as an argument
ESP8266WebServer server(80);
//holds the current upload
File UploadFile;
String fileName;

//-------------- FSBrowser application -----------
//format bytes
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

String getContentType(String filename) {
  if (server.hasArg("download")) return "application/octet-stream";
  //if(filename.endsWith(".src")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

void handleFileCreate() {
  if (server.args() == 0)
    return server.send(500, "text/plain", "BAD ARGS");
  String path = server.arg(0);
  if (!path.startsWith("/")) path = "/" + path;
  DBG_OUTPUT_PORT.println("handleFileCreate: " + path);
  if (path == "/")
    return server.send(500, "text/plain", "BAD PATH");
  if (SPIFFS.exists(path))
    return server.send(500, "text/plain", "FILE EXISTS");
  File file = SPIFFS.open(path, "w");
  if (file)
    file.close();
  else
    return server.send(500, "text/plain", "CREATE FAILED");
  server.send(200, "text/plain", "");
  path = String();
}

void handleFileDelete() {
  if (server.args() == 0) return server.send(500, "text/plain", "BAD ARGS");
  String path = server.arg(0);
  if (!path.startsWith("/")) path = "/" + path;
  DBG_OUTPUT_PORT.println("handleFileDelete: " + path);
  if (path == "/")
    return server.send(500, "text/plain", "BAD PATH");
  if (!SPIFFS.exists(path))
    return server.send(404, "text/plain", "FileNotFound");
  SPIFFS.remove(path);
  server.send(200, "text/plain", "");
  path = String();
}

// An empty ESP8266 Flash ROM must be formatted before using it, actual a problem
void handleFormat() {
  DBG_OUTPUT_PORT.println("Format SPIFFS");
  if (SPIFFS.format()) {
    if (!SPIFFS.begin()) {
      DBG_OUTPUT_PORT.println("Format SPIFFS failed");
    }
  } else {
    DBG_OUTPUT_PORT.println("Format SPIFFS failed");
  }
  if (!SPIFFS.begin()) {
    DBG_OUTPUT_PORT.println("SPIFFS failed, needs formatting");
  } else {
    DBG_OUTPUT_PORT.println("SPIFFS mounted");
  }
}

void handleFileList() {
  if (!server.hasArg("dir")) {
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }

  String path = server.arg("dir");
  DBG_OUTPUT_PORT.println("handleFileList: " + path);
  Dir dir = SPIFFS.openDir(path);
  path = String();

  String output = "[";
  while (dir.next()) {
    File entry = dir.openFile("r");
    if (output != "[") output += ',';
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += String(entry.name()).substring(1);
    output += "\"}";
    entry.close();
  }

  output += "]";
  server.send(200, "text/json", output);
}

bool handleFileRead(String path) {
  DBG_OUTPUT_PORT.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    size_t contentLength = file.size();

    file.close();
    return true;
  }
  return false;
}

//------------------- WiFi Car application -----------
void handleRoot() {
  String message = "<!DOCTYPE html><html><head>";
  message += "</head><body>";
  message += "<div id=\"block\" style=\"font-size:24pt\">";
  message += "<a href='#' onclick='move(\"f\");'>forward</a><BR/>";
  message += "<a href='#' onclick='move(\"b\");'>backwards</a><BR/>";
  message += "<a href='#' onclick='move(\"l\");'>left</a><BR/>";
  message += "<a href='#' onclick='move(\"r\");'>right</a><BR/>";
  message += "<p id=\"dmEvent\">Acc</p>";
  message += "<div id=\"vector\"</div>";
  message += "</div>";
  // if Access Point mode, get Javascript file from Flash File System
  #ifdef ACCESSPOINT
    message += "<script type='text/javascript' src='/wifi-carAP.js'></script>";
  #else
    message += "<script type='text/javascript' src='http://www.rudiswiki.de/wifi-carAP.js'></script>";
  #endif
  message += "</body></html>";
  message += "\n";

  server.send ( 200, "text/html", message );

  analogWrite(5, 0);
  analogWrite(4, 0);
  digitalWrite(0, 1);
  digitalWrite(2, 1);
}

void handleMotor() {
  String leftText = server.arg(0);
  String rightText = server.arg(1);
  DBG_OUTPUT_PORT.println("[" + leftText + "][" + rightText + "]");
  int left = leftText.toInt();
  int right = rightText.toInt();
  if (left < 0) {
    motorAForward = 0;
  } else {
    motorAForward = 1;
  }
  if (right < 0) {
    motorBForward = 0;
  } else {
    motorBForward = 1;
  }
  analogWrite(5, abs(left));      // was left
  analogWrite(4, abs(right));     // was right
  digitalWrite(0, motorBForward); // was A
  digitalWrite(2, motorAForward); // was B

  delay(200);  // every 200 ms a new motion value
  analogWrite(5, 0);
  analogWrite(4, 0);
  digitalWrite(0, 1);
  digitalWrite(2, 1);

  String message = "OK";
  server.send ( 200, "text/html", message );
}

//------------------ setup ---------------
void setup() {
  DBG_OUTPUT_PORT.begin(115200);
  DBG_OUTPUT_PORT.print("\n");
  DBG_OUTPUT_PORT.setDebugOutput(true);
  /// LED_user setup and test, for debug purpose.
  pinMode(BUILTIN_LED2, OUTPUT);
  digitalWrite(BUILTIN_LED2, LOW);
  delay(100); // ms
  digitalWrite(BUILTIN_LED2, HIGH);
  delay(300); // ms, pause because of AP mode

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);

  // Connect to WiFi network

#ifdef ACCESSPOINT
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
#else
  // Station, access to router
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DBG_OUTPUT_PORT.print(".");
  }
  DBG_OUTPUT_PORT.println("");
  DBG_OUTPUT_PORT.println("WiFi connected");
  // Print the IP address
  DBG_OUTPUT_PORT.println(WiFi.localIP());
#endif

  //---------------- Server init
  // list directory
  server.on("/list", HTTP_GET, handleFileList);
  // create file
  server.on("/create", handleFileCreate);
  //delete file
  server.on("/delete", HTTP_GET, handleFileDelete);
  // called after file upload
  //server.on("/edit", HTTP_POST, [](){ server.send(200, "text/plain", ""); });

  // Format Flash ROM - too dangerous!
  // server.on ( "/format", handleFormat );
  
  // get filename from client for upload; setup is also handle
  server.on("/upload", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/html", serverIndex);
  });

  server.onFileUpload([]() {  // called several times from Parsing.cpp while upload
    if (server.uri() != "/update") return;
    // get file
    HTTPUpload& upload = server.upload();
    Serial.print("status: "); 
    Serial.println( (int)upload.status);  // need 2 commands to work!
    if (upload.status == UPLOAD_FILE_START) {
      fileName = upload.filename;
      Serial.println("Upload Name: " + fileName);
      String path = "/" + fileName;
      UploadFile = SPIFFS.open(path, "w");
      // already existing file will be overwritten!
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (UploadFile)
        UploadFile.write(upload.buf, upload.currentSize);
      Serial.println(fileName + " size: " + upload.currentSize);
    } else if (upload.status == UPLOAD_FILE_END) {
      Serial.print("Upload Size: ");
      Serial.println(upload.totalSize);  // need 2 commands to work!
      if (UploadFile)
        UploadFile.close();
    }
    yield();
  });

  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    // ESP.restart();  // not needed
    DBG_OUTPUT_PORT.println("dir SPIFFS");
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      DBG_OUTPUT_PORT.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
  });

  //called when the url is not defined here
  //use it to load content from SPIFFS
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  server.on ( "/", handleRoot );
  server.on ( "/index.html", handleRoot );
  server.on ( "/engines", handleMotor );

  //--------------- Start the server
  server.begin();
  DBG_OUTPUT_PORT.println("Server started");

  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);

  digitalWrite(5, 0);
  digitalWrite(4, 0);

  digitalWrite(0, 1);
  digitalWrite(2, 1);

  // show the SPIFFS contents, name and size
  if (!SPIFFS.begin()) {
    DBG_OUTPUT_PORT.println("SPIFFS failed, needs formatting");
  } else {
    DBG_OUTPUT_PORT.println("dir SPIFFS");
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      DBG_OUTPUT_PORT.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    DBG_OUTPUT_PORT.printf("\n");
    FSInfo fs_info;
    if (!SPIFFS.info(fs_info)) {
      Serial.println("fs_info failed");
    } else {
      Serial.printf("Total: %u\nUsed: %u\nBlock: %u\nPage: %u\nMax open files: %u\nMax path len: %u\n",
                    fs_info.totalBytes,
                    fs_info.usedBytes,
                    fs_info.blockSize,
                    fs_info.pageSize,
                    fs_info.maxOpenFiles,
                    fs_info.maxPathLength
                   );
    }
  }
  // show READY for use
  digitalWrite(BUILTIN_LED2, LOW);
  delay(300); // ms
  digitalWrite(BUILTIN_LED2, HIGH);
}

void loop() {
  server.handleClient();
  delay(1);
}


