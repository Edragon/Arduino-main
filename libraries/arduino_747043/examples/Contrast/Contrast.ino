#include "Nokia_5110.h"

#define RST 2
#define CE 3
#define DC 4
#define DIN 5
#define CLK 6

Nokia_5110 lcd = Nokia_5110(RST, CE, DC, DIN, CLK);


void setup() {
    lcd.setContrast(60); // 60 is the default value set by the driver
}

void loop() {
    lcd.clear();
    lcd.println("`setContrast` method lets you change the LCD's Contrast.");
    delay(8000);

    lcd.clear();
    lcd.println("It accepts a value between 0 and 127 (0 >= value <= 127)");
    delay(8000);
}
