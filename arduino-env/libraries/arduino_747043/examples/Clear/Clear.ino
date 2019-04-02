#include "Nokia_5110.h"

#define RST 2
#define CE 3
#define DC 4
#define DIN 5
#define CLK 6

Nokia_5110 lcd = Nokia_5110(RST, CE, DC, DIN, CLK);

void setup() {
    lcd.setCursor(5, 2);
	lcd.print("Clear methods");
	delay(4000);

	/**
	 * Clear method with no parameter, clears the whole sceen and then sets the cursor to the first row and first column. X=0, Y=0 .
	 */
	lcd.clear(); // One way of using clear method.

	lcd.setCursor(15, 2);
    lcd.print("Timer");

	lcd.setCursor(15, 3);
    lcd.print("00 : 00 : 00");
}


void hours(){
    static unsigned int hours = 0;

    if(hours <= 22){
        hours++;
    }else{
        hours = 0;
    }

    lcd.clear(3, 15, 25); // This is another way of using clear method. It only clears a portion of the screen
    lcd.print(hours);
}

void minutes(){
    static unsigned int minutes = 0;

    if(minutes <= 58){
        minutes++;
    }else{
        minutes = 0;
        hours();
    }

    lcd.clear(3, 36, 46); // This is another way of using clear method. It only clears a portion of the screen
    lcd.print(minutes);
}

void seconds(){
    static unsigned int seconds = 0;

    if(seconds <= 58) {
        seconds++;
    }else{
        seconds = 0;
        minutes();
    }

    lcd.clear(3, 57, 70); // This is another way of using clear method. It only clears a portion of the screen
    lcd.print(seconds);
}

void timer(){
	static unsigned long timerState = 0;
	unsigned long timer = (millis() / 1000);
    if(timer != timerState){
        timerState = timer;
        seconds();
    }
}

void loop() {
    timer();
}
