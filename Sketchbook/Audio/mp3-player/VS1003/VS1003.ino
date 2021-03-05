/*
for uno sd card should connect to 11 (MOSI), 12 (MISO), 13 (SCK)
 for 2560 it should be 51 (MOSI), 50 (MISO), 52 (SCK)
 */
 
 
#include <SD.h>
#include <SPI.h>
 
//for sd test
File root;
int i=0;
int fileNumber=0;
 
//set vs1003 pins
int xCs=9;
int xReset=8;
int dreq= 7;
int xDcs=6;
 
int sspin=53; //sspin for mega
 
int volume=0x30;//set volume here 
int DREQ=digitalRead(dreq);
 
char* playlist[]={
};
 
File mp3;
 
void setup(){
  delay(10);
  //set 1003
  SPI.begin();
  SPI.setBitOrder(MSBFIRST); //send most-significant bit first
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV16); 
  pinMode(7,INPUT);
  pinMode(8,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(9,OUTPUT);
  digitalWrite(sspin,HIGH);
  Mp3Reset();
 
  Serial.begin(9600);
  Serial.print("Set SD card..."); 
  while (!SD.begin()){   
  }
  Serial.println("sd ready."); 
  root = SD.open("/");
  printDirectory(root, 0);
  Serial.print("Totally ");
  Serial.print(fileNumber);
  Serial.print(" file(s)");
  Serial.println();
}
 
void loop(){
   play("s1.mp3");
  play("s2.mp3");
  play("s3.mp3");
  play("s4.mp3");
  play("s5.mp3");
  play("s6.mp3");
  play("s8.mp3");
  play("s9.mp3");
  play("s10.mp3");
  play("s11.mp3");
  play("s12.mp3");
  play("s13.mp3");
  play("s14.mp3");
 
}
 
void play(char* playplay){
  int val,i;
  mp3=SD.open(playplay);
  delay(10);
  for(i=0;i<548;i++){
    digitalWrite(sspin,HIGH);
    val =mp3.read();
    digitalWrite(xDcs,LOW);
    SPI.transfer(val);
    digitalWrite(xDcs,HIGH);
    digitalWrite(sspin,LOW);
  }
  if(DREQ==HIGH){
    while(mp3.available()){
      for(i=0;i<32;i++){
        digitalWrite(sspin,HIGH);
        val =mp3.read();
        digitalWrite(xDcs,LOW);
        SPI.transfer(val);
        digitalWrite(xDcs,HIGH);
        digitalWrite(sspin,LOW);
      }
      delayMicroseconds(35);
    }
  }
  mp3.close();
}
void Mp3Reset(){
  digitalWrite(xReset,LOW);
  delay(100);
  digitalWrite(xCs,HIGH);
  digitalWrite(xDcs,HIGH);
  digitalWrite(xReset,HIGH);
  commad(0X00,0X08,0X04); //write into mode
  delay(10);
  if(DREQ==HIGH){
    commad(0X03,0XC0,0X00);//set VS1003 clock
    delay(10);
    commad(0X05,0XBB,0X81);//set VS1003 sample rate at 44kps stero
    delay(10);
    commad(0X02,0X00,0X55);//set duplicate sound? 重音
    delay(10);
    commad(0X0B,volume,volume);//highest volumn at 0x0000, and lowest at 0xFEFE
    delay(10); 
    SPI.transfer(0);
    SPI.transfer(0);
    SPI.transfer(0);        
    SPI.transfer(0);
    digitalWrite(xCs,HIGH);
    digitalWrite(xReset,HIGH);
    digitalWrite(xDcs,HIGH);
    digitalWrite(sspin,LOW);
  }
}
void commad(unsigned char addr,unsigned char hdat,unsigned char ldat )
{  
  int DREQ=digitalRead(dreq);
  if(DREQ==HIGH){
    digitalWrite(xCs,LOW);
    SPI.transfer(0X02);
    SPI.transfer(addr);
    SPI.transfer(hdat);
    SPI.transfer(ldat);    
    digitalWrite(xCs,HIGH);
  }
}
void printDirectory(File dir, int numTabs) {
  while(i<6){
    File entry =  dir.openNextFile();
    if (! entry) { // no more files
      i++;
    }
    for (uint8_t i=0; i<numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()){
      Serial.println("/");
      printDirectory(entry, numTabs+1);
    }
    else if(entry.size()) {// files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
      fileNumber++;
    }
  }
}
