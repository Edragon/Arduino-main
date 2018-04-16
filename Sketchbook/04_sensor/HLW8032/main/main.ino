#include <SoftwareSerial.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySerial(10, 11); // RX, TX

LiquidCrystal_I2C lcd(0x27, 20, 4);


bool read_good = false;
int led = 13; //indication LED
int hlw[23];

char buff[50];
char buff2[50];
char buff3[50];

void setup()
{

  lcd.init();
  lcd.backlight();
  
  Serial.begin(9600);   // Open serial communications and wait for port to open:
  mySerial.begin(4800);
  pinMode (led, OUTPUT);
  delay(500);
}


int once_flag = 1; //to make sure the loop runs only once

void loop() // run over and over
{
  if ( once_flag == 0) {
    Serial.println("print once demo .. ");
    once_flag = 2;
    if (read_data ()) {
      //print_raw_data ();
      print_parse ();
      process_data () ;
    }
  }

  else if ( once_flag == 1) {
    Serial.println("keep looping data .. ");
    if (read_data ()) {
      print_parse ();
      process_data () ;
      delay (1000);
      lcd.clear();
    } else {
      Serial.println("error read");
      delay (1000);
    }
  }

  else if ( once_flag == 2) {

  }
}

