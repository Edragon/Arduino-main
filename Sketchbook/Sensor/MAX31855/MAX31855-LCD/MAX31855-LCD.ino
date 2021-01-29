#include "Adafruit_MAX31855.h"
#include <LiquidCrystal.h>

int thermoCLK = 3;
int thermoCS = 4;
int thermoDO = 5;

// Initialize the Thermocouple
Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);  
  lcd.print("MAX31855 Init");
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp
   lcd.setCursor(0, 0);
   lcd.print("www.devacron.com");
   lcd.print("  "); 

   double c = thermocouple.readCelsius();
   lcd.setCursor(0, 1);
   if (isnan(c)) 
   {
     lcd.print("T/C Problem");
   } 
   else 
   {
     lcd.print("Temp = "); 
     lcd.print(c);
     lcd.print("c"); 
   }

   delay(1000);
}
