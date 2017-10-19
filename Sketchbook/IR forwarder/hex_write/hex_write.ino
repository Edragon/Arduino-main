//#include <ESP8266WiFi.h>
//#include <FS.h>

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

byte readByte;
byte correct = 0x00;
byte wrong = 0xFF;
byte header = 0xFA;
int ender = 237;

String readString;

void listen();
void FA_buffer();
void cmd_buffer();

void sendhex();
void quit();
void print_get();

unsigned int ary_get[200];
unsigned char ary_sort[120];
boolean check = false;

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  Serial.println("linstening");
  listen();
  FA_buffer();
  delay(5000);
  
  quit();
  print_get();
  delay(500);
  //sendhex();
}
