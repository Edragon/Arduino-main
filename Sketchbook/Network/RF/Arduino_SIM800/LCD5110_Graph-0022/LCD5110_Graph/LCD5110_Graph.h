/*
  LCD5110_Graph.h - Arduino library support for Nokia 5110 compatible LCDs
  Copyright (C)2011 Henning Karlsen. All right reserved
  
  Basic functionality of this library are based on the demo-code provided by
  ITead studio. You can find the latest version of the library at
  http://www.henningkarlsen.com/electronics

  This library has been made to make it easy to use the Nokia 5110 LCD module 
  as a graphics display on an Arduino.

  If you make any modifications or improvements to the code, I would appreciate
  that you share the code with me so that I might include it in the next release.
  I can be contacted through http://www.henningkarlsen.com/electronics/contact.php

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

#ifndef LCD5110_Graph_h
#define LCD5110_Graph_h

#define LEFT 0
#define RIGHT 9999
#define CENTER 9998

#define LCD_COMMAND 0
#define LCD_DATA 1

#include "WProgram.h"

struct _current_font
{
	uint8_t* font;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t offset;
	uint8_t numchars;
};

class LCD5110
{
	public:
		LCD5110(int SCK, int MOSI, int DC, int RST, int CS);
		void InitLCD();
		void update();
		void clrScr();
		void invert(bool mode);
		void setPixel(uint16_t x, uint16_t y);
		void clrPixel(uint16_t x, uint16_t y);
		void invPixel(uint16_t x, uint16_t y);
		void print(char *st, int x, int y);
		void printNumI(long num, int x, int y);
		void printNumF(double num, byte dec, int x, int y);
		void setFont(uint8_t* font);
		void drawBitmap(int x, int y, uint8_t* bitmap, int sx, int sy, bool flash=true);
		void drawLine(int x1, int y1, int x2, int y2);
		void drawRect(int x1, int y1, int x2, int y2);
		void drawRoundRect(int x1, int y1, int x2, int y2);
		void drawCircle(int x, int y, int radius);

	protected:
		volatile uint8_t *P_SCK, *P_MOSI, *P_DC, *P_RST, *P_CS;
		volatile uint8_t B_SCK, B_MOSI, B_DC, B_RST, B_CS;
		_current_font	cfont;
		uint8_t	scrbuf[504];

		void _LCD_Write(unsigned char data, unsigned char mode);
		void _print_char(unsigned char c, int x, int row);
		void drawHLine(int x, int y, int l);
		void drawVLine(int x, int y, int l);
};

#endif