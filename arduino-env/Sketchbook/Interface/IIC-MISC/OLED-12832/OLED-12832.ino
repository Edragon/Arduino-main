#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <EEPROM.h>
int address = 0;
byte value;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();

  write_random_eeprom(512);
  eeprom_clean(5); // clear first 20bytes address
}


void loop() {
  
  eeprom_read_print(512);

}

void eeprom_read_print(int max_addr) {

  if (address == max_addr) {
    address = 0;
  }
  for (address = 0; address <= max_addr; address++) {

    value = EEPROM.read(address);

    // address
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("ADDR:  ");

    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.println(address);

    // value
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print("VAL:   ");

    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.println(value, HEX);

    display.display();
    delay(500);
    display.clearDisplay();

  }
}



void eeprom_clean(int clr_addr)
{
  if (address == clr_addr) {
    address = 0;
  }
  for (address = 0; address <= clr_addr; address++) {
    EEPROM.write(address, 4);

    // turn the LED on when we're done
    digitalWrite(13, HIGH);
  }

}

void write_random_eeprom (int write_addr)
{
  if (address == write_addr) {
    address = 0;
  }
  for (address = 0; address <= write_addr; address++) {
    int val = analogRead(0) / 4;
    EEPROM.write(address, val);
    delay(10);
  }
}
