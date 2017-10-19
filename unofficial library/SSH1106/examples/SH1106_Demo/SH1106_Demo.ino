#include <SPI.h>
#include "SH1106_SPI.h"

#define USE_FRAME_BUFFER

#ifdef USE_FRAME_BUFFER
SH1106_SPI_FB lcd;
#else
SH1106_SPI lcd;
#endif

void setup(void)
{
	Serial.begin(9600);
	lcd.begin();
	lcd.print(F("Preparing benchmark"));
#ifdef USE_FRAME_BUFFER
	lcd.renderAll();
#endif
	delay(1000);
}

uint8_t bmp[] = {
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 
};

	int i = 0;
	char *str = "01234567890123456789012";
void loop(void) 
{
	size_t len;
	unsigned long time = micros();
	lcd.clear();
	time = micros() - time;
	lcd.print(F("The time it clear the screen is: "));
	lcd.print(time);
	Serial.print(F("The time it clear the screen is: "));
	Serial.println(time);
#ifdef USE_FRAME_BUFFER
	lcd.renderAll();
#endif
	delay(3000);
	
	lcd.clear();
	time = micros();
#ifdef USE_FRAME_BUFFER
	len = lcd.print(F("!\"#$%'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz !\"#$%'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrs"));
	lcd.renderString(0, 0, len);
#else
	len = lcd.print(F("!\"#$%'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz !\"#$%'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopq"));
#endif
	time = micros() - time;
	delay(3000);

	lcd.clear();
	lcd.print(F("The time it took to print "));
	lcd.print(len);
	lcd.print(F(" chars is: "));
	lcd.print(time);
	Serial.print(F("The time it took to print "));
	Serial.print(len);
	Serial.print(F(" chars is: "));
	Serial.println(time);
#ifdef USE_FRAME_BUFFER
	lcd.renderAll();
#endif
	delay(3000);

	lcd.clear();
	lcd.gotoXY(5,3);
	lcd.print(F("Test gotoXY"));
#ifdef USE_FRAME_BUFFER
	lcd.renderAll();
#endif
	delay(3000);

	lcd.clear();
	time = micros();
	lcd.writeBitmap(bmp, 10, 2, 25, 3);
#ifdef USE_FRAME_BUFFER
	lcd.renderAll();
#endif
	time = micros() - time;
	delay(3000);

	lcd.clear();
	lcd.print(F("The time it took to draw a 25x3 (25x18) bitmap is: "));
	lcd.print(time);
	Serial.print(F("The time it took to draw a 25x3 (25x18) bitmap is: "));
	Serial.println(time);
#ifdef USE_FRAME_BUFFER
	lcd.renderAll();
#endif
	delay(3000);
	
#ifdef USE_FRAME_BUFFER
	lcd.clear();
	time = micros();
	lcd.writeRect(5, 5, 50, 40);
	lcd.writeLine(75, 3, 75, 35);
	lcd.writeLine(60, 10, 60, 40);
	lcd.writeLine(10, 47, 60, 47);
	lcd.renderAll();
	time = micros() - time;
	delay(3000);

	lcd.clear();
	len = lcd.print(F("The time it took draw a rect and 3 lines: "));
	len += lcd.print(time);
	Serial.print(F("The time it took draw a rect and 3 lines: "));
	Serial.println(time);
	lcd.renderString(0, 0, len);
	delay(3000);

	lcd.clear();
	time = micros();
	for (uint8_t row = 0; row < SH1106_Y_PIXELS; row++)
	{
		for (uint8_t col = 0; col < SH1106_X_PIXELS; col++)
		{
			uint8_t pixel = (col + row) % 2;
			lcd.setPixel(col, row, pixel);
		}
	}
	lcd.renderAll();
	time = micros() - time;
	delay(5000);

	lcd.clear();
	lcd.print(F("The time it took to run setPixel on all 8192 pixels and render it: "));
	lcd.print(time);
	Serial.print(F("The time it took to run setPixel on all 8192 pixels and render it: "));
	Serial.println(time);
	lcd.renderAll();
	delay(5000);
#endif
}