#include <TFT.h>  // Arduino LCD library
#include <SPI.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX , only use 2 to read 8032

// pin definition for the Uno
#define cs   8
#define dc   10
#define rst  9

bool read_good = false;
int led = 13; //indication LED
int hlw[23];

char buff[50];
char buff1[50];
char buff2[50];
char buff3[50];
char buff4[50];
char buff5[50];
char volt_temp[7], current_temp[7], pwr_temp[7], pulse_temp[7], pwr_used_temp[7];
char Printout[4];

TFT TFTscreen = TFT(cs, dc, rst);

void setup() {
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);

  Serial.begin(9600);   // Open serial communications and wait for port to open:
  mySerial.begin(4800);
  pinMode (led, OUTPUT);
  delay(500);
}



void loop() {
  if (read_data ()) {
    process_data_2 ();
    HLW8032_DATA_PRINT();
  }

}
