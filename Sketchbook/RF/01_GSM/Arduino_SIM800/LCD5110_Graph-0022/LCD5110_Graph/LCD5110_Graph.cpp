/*
  LCD5110_Graph.cpp - Arduino library support for Nokia 5110 compatible LCDs
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

#include "WProgram.h"
#include "LCD5110_Graph.h"
#include <avr/pgmspace.h>
#include <pins_arduino.h>

LCD5110::LCD5110(int SCK, int MOSI, int DC, int RST, int CS)
{ 
	P_SCK	= portOutputRegister(digitalPinToPort(SCK));
	B_SCK	= digitalPinToBitMask(SCK);
	P_MOSI	= portOutputRegister(digitalPinToPort(MOSI));
	B_MOSI	= digitalPinToBitMask(MOSI);
	P_DC	= portOutputRegister(digitalPinToPort(DC));
	B_DC	= digitalPinToBitMask(DC);
	P_RST	= portOutputRegister(digitalPinToPort(RST));
	B_RST	= digitalPinToBitMask(RST);
	P_CS	= portOutputRegister(digitalPinToPort(CS));
	B_CS	= digitalPinToBitMask(CS);
	pinMode(SCK,OUTPUT);
	pinMode(MOSI,OUTPUT);
	pinMode(DC,OUTPUT);
	pinMode(RST,OUTPUT);
	pinMode(CS,OUTPUT);
}

void LCD5110::_LCD_Write(unsigned char data, unsigned char mode)
{   
	*P_CS &= ~B_CS;

	if (mode==0)
		*P_DC &= ~B_DC;
	else
		*P_DC |= B_DC;

	for (unsigned char c=0; c<8; c++)
	{
		if (data & 0x80)
			*P_MOSI |= B_MOSI;
		else
			*P_MOSI &= ~B_MOSI;
		data = data<<1;
		*P_SCK &= ~B_SCK;
		asm ("nop");
		*P_SCK |= B_SCK;
	}

	*P_CS |= B_CS;
}

void LCD5110::InitLCD()
{
	*P_DC |= B_DC;
	*P_MOSI |= B_MOSI;
	*P_SCK |= B_SCK;
	*P_CS |= B_CS;

	*P_RST &= ~B_RST;
	delay(10);
	*P_RST |= B_RST;

	_LCD_Write(0x21, LCD_COMMAND);
	_LCD_Write(0xC6, LCD_COMMAND);
	_LCD_Write(0x06, LCD_COMMAND);
	_LCD_Write(0x13, LCD_COMMAND);
	_LCD_Write(0x20, LCD_COMMAND);
	_LCD_Write(0x40, LCD_COMMAND);
	_LCD_Write(0x80, LCD_COMMAND);
	_LCD_Write(0x0C, LCD_COMMAND);

	clrScr();
	update();
	cfont.font=0;
}

void LCD5110::update()
{
	_LCD_Write(0x40, LCD_COMMAND);
	_LCD_Write(0x80, LCD_COMMAND);
	for (int b=0; b<504; b++)
		_LCD_Write(scrbuf[b], LCD_DATA);
}

void LCD5110::clrScr()
{
	for (int c=0; c<504; c++)
		scrbuf[c]=0;
}

void LCD5110::invert(bool mode)
{
	if (mode==true)
		_LCD_Write(0x0D, LCD_COMMAND);
	else
		_LCD_Write(0x0C, LCD_COMMAND);
}

void LCD5110::setPixel(uint16_t x, uint16_t y)
{
	int by, bi;

	if ((x>=0) and (x<84) and (y>=0) and (y<48))
	{
		by=((y/8)*84)+x;
		bi=y % 8;

		scrbuf[by]=scrbuf[by] | (1<<bi);
	}
}

void LCD5110::clrPixel(uint16_t x, uint16_t y)
{
	int by, bi;

	if ((x>=0) and (x<84) and (y>=0) and (y<48))
	{
		by=((y/8)*84)+x;
		bi=y % 8;

		scrbuf[by]=scrbuf[by] & ~(1<<bi);
	}
}

void LCD5110::invPixel(uint16_t x, uint16_t y)
{
	int by, bi;

	if ((x>=0) and (x<84) and (y>=0) and (y<48))
	{
		by=((y/8)*84)+x;
		bi=y % 8;

		if ((scrbuf[by] & (1<<bi))==0)
			scrbuf[by]=scrbuf[by] | (1<<bi);
		else
			scrbuf[by]=scrbuf[by] & ~(1<<bi);
	}
}

void LCD5110::print(char *st, int x, int y)
{
	unsigned char ch;
	int stl;

	stl = strlen(st);
	if (x == RIGHT)
		x = 84-(stl*cfont.x_size);
	if (x == CENTER)
		x = (84-(stl*cfont.x_size))/2;

	for (int cnt=0; cnt<stl; cnt++)
			_print_char(*st++, x + (cnt*(cfont.x_size)), y);
}

void LCD5110::printNumI(long num, int x, int y)
{
  char buf[13];
  char st[15];
  boolean neg=false;
  int c=0;
  
  if (num==0)
  {
	  st[0]=48;
	  st[1]=0;
  }
  else
  {
	  if (num<0)
	  {
		neg=true;
		num=-num;
	  }
	  
	  while (num>0)
	  {
		buf[c]=48+(num % 10);
		c++;
		num=(num-(num % 10))/10;
	  }
	  buf[c]=0;
	  
	  if (neg)
	  {
		st[0]=45;
	  }
	  
	  for (int i=0; i<c; i++)
	  {
		st[i+neg]=buf[c-i-1];
	  }
	  st[c+neg]=0;
  }

  print(st,x,y);
}

void LCD5110::printNumF(double num, byte dec, int x, int y)
{
  char buf[25];
  char st[27];
  boolean neg=false;
  int c=0;
  int c2;
  unsigned long inum;
  
  if (num==0)
  {
	  st[0]=48;
	  st[1]=46;
	  for (int i=0; i<dec; i++)
		  st[2+i]=48;
	  st[2+dec]=0;
  }
  else
  {
	  if (num<0)
	  {
		neg=true;
		num=-num;
	  }
	  
	  if (dec<1)
		dec=1;
	  if (dec>5)
		dec=5;
	  
	  inum=long(num*pow(10,dec));
	  
	  while (inum>0)
	  {
		buf[c]=48+(inum % 10);
		c++;
		inum=(inum-(inum % 10))/10;
	  }
	  if ((num<1) and (num>0))
	  {
		  buf[c]=48;
		  c++;
	  }
	  buf[c]=0;
	  
	  if (neg)
	  {
		st[0]=45;
	  }
	  
	  c2=neg;
	  for (int i=0; i<c; i++)
	  {
		st[c2]=buf[c-i-1];
		c2++;
		if ((c-(c2-neg))==dec)
		{
		  st[c2]=46;
		  c2++;
		}
	  }
	  st[c2]=0;
  }

  print(st,x,y);
}

void LCD5110::_print_char(unsigned char c, int x, int y)
{
	if ((cfont.y_size % 8) == 0)
	{
		int font_idx = ((c - cfont.offset)*(cfont.x_size*(cfont.y_size/8)))+4;
		for (int rowcnt=0; rowcnt<(cfont.y_size/8); rowcnt++)
		{
			for(int cnt=0; cnt<cfont.x_size; cnt++)
			{
				for (int b=0; b<8; b++)
					if ((pgm_read_byte(&cfont.font[font_idx+cnt+(rowcnt*cfont.x_size)]) & (1<<b))!=0)
						setPixel(x+cnt, y+(rowcnt*8)+b);
					else
						clrPixel(x+cnt, y+(rowcnt*8)+b);
			}
		}
	}
	else
	{
		int font_idx = ((c - cfont.offset)*((cfont.x_size*cfont.y_size/8)))+4;
		int cbyte=pgm_read_byte(&cfont.font[font_idx]);
		int cbit=7;
		for (int cx=0; cx<cfont.x_size; cx++)
		{
			for (int cy=0; cy<cfont.y_size; cy++)
			{
				if ((cbyte & (1<<cbit)) != 0)
					setPixel(x+cx, y+cy);
				else
					clrPixel(x+cx, y+cy);
				cbit--;
				if (cbit<0)
				{
					cbit=7;
					font_idx++;
					cbyte=pgm_read_byte(&cfont.font[font_idx]);
				}
			}
		}
	}
}

void LCD5110::setFont(uint8_t* font)
{
	cfont.font=font;
	cfont.x_size=pgm_read_byte(&cfont.font[0]);
	cfont.y_size=pgm_read_byte(&cfont.font[1]);
	cfont.offset=pgm_read_byte(&cfont.font[2]);
	cfont.numchars=pgm_read_byte(&cfont.font[3]);
}

void LCD5110::drawHLine(int x, int y, int l)
{
	int by, bi;

	if ((x>=0) and (x<84) and (y>=0) and (y<48))
	{
		for (int cx=0; cx<l; cx++)
		{
			by=((y/8)*84)+x;
			bi=y % 8;

			scrbuf[by+cx]=scrbuf[by+cx] | (1<<bi);
		}
	}
}

void LCD5110::drawVLine(int x, int y, int l)
{
	int by, bi;

	if ((x>=0) and (x<84) and (y>=0) and (y<48))
	{
		for (int cy=0; cy<l; cy++)
		{
			setPixel(x, y+cy);
		}
	}
}

void LCD5110::drawLine(int x1, int y1, int x2, int y2)
{
	int tmp;
	double delta, tx, ty;
	double m, b, dx, dy;
	
	if (((x2-x1)<0))
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
		tmp=y1;
		y1=y2;
		y2=tmp;
	}
    if (((y2-y1)<0))
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
		tmp=y1;
		y1=y2;
		y2=tmp;
	}

	if (y1==y2)
	{
		if (x1>x2)
		{
			tmp=x1;
			x1=x2;
			x2=tmp;
		}
		drawHLine(x1, y1, x2-x1);
	}
	else if (x1==x2)
	{
		if (y1>y2)
		{
			tmp=y1;
			y1=y2;
			y2=tmp;
		}
		drawVLine(x1, y1, y2-y1);
	}
	else if (abs(x2-x1)>abs(y2-y1))
	{
		delta=(double(y2-y1)/double(x2-x1));
		ty=double(y1);
		if (x1>x2)
		{
			for (int i=x1; i>=x2; i--)
			{
				setPixel(i, int(ty+0.5));
        		ty=ty-delta;
			}
		}
		else
		{
			for (int i=x1; i<=x2; i++)
			{
				setPixel(i, int(ty+0.5));
        		ty=ty+delta;
			}
		}
	}
	else
	{
		delta=(float(x2-x1)/float(y2-y1));
		tx=float(x1);
        if (y1>y2)
        {
			for (int i=y2+1; i>y1; i--)
			{
		 		setPixel(int(tx+0.5), i);
        		tx=tx+delta;
			}
        }
        else
        {
			for (int i=y1; i<y2+1; i++)
			{
		 		setPixel(int(tx+0.5), i);
        		tx=tx+delta;
			}
        }
	}

}

void LCD5110::drawRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
	}
	if (y1>y2)
	{
		tmp=y1;
		y1=y2;
		y2=tmp;
	}

	drawHLine(x1, y1, x2-x1);
	drawHLine(x1, y2, x2-x1);
	drawVLine(x1, y1, y2-y1);
	drawVLine(x2, y1, y2-y1+1);
}

void LCD5110::drawRoundRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
	}
	if (y1>y2)
	{
		tmp=y1;
		y1=y2;
		y2=tmp;
	}
	if ((x2-x1)>4 && (y2-y1)>4)
	{
		setPixel(x1+1,y1+1);
		setPixel(x2-1,y1+1);
		setPixel(x1+1,y2-1);
		setPixel(x2-1,y2-1);
		drawHLine(x1+2, y1, x2-x1-3);
		drawHLine(x1+2, y2, x2-x1-3);
		drawVLine(x1, y1+2, y2-y1-3);
		drawVLine(x2, y1+2, y2-y1-3);
	}
}

void LCD5110::drawCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
	char ch, cl;
	
	setPixel(x, y + radius);
	setPixel(x, y - radius);
	setPixel(x + radius, y);
	setPixel(x - radius, y);
 
	while(x1 < y1)
	{
		if(f >= 0) 
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;    
		setPixel(x + x1, y + y1);
		setPixel(x - x1, y + y1);
		setPixel(x + x1, y - y1);
		setPixel(x - x1, y - y1);
		setPixel(x + y1, y + x1);
		setPixel(x - y1, y + x1);
		setPixel(x + y1, y - x1);
		setPixel(x - y1, y - x1);
	}
}

void LCD5110::drawBitmap(int x, int y, uint8_t* bitmap, int sx, int sy, bool flash)
{
	int bit;
	byte data;

	for (int cy=0; cy<sy; cy++)
	{
		bit= cy % 8;
		for(int cx=0; cx<sx; cx++)
		{
			if (flash)
			data=pgm_read_byte(&bitmap[cx+((cy/8)*sx)]);
				else
			data=bitmap[cx+((cy/8)*sx)];

			if ((data & (1<<bit))>0)
				setPixel(x+cx, y+cy);
			else
				clrPixel(x+cx, y+cy);
		}
	}      
}
