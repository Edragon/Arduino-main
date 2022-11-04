#include "SH1106_SPI.h"
#include <SPI.h>

SH1106_SPI_FB::SH1106_SPI_FB()
{
}

void SH1106_SPI_FB::begin(bool invert, bool fastSpi, uint8_t contrast, uint8_t Vpp)
{
	SH1106_PORT |= (PIN_DC | PIN_RESET | PIN_CS);
	SH1106_DDR |= (PIN_DC | PIN_RESET | PIN_CS);
	SPI.begin();
	if (fastSpi)
		SPI.setClockDivider(SPI_CLOCK_DIV2);
	
	// LCD init section:
	
	uint8_t invertSetting = invert ? 0xA7 : 0xA6;
	Vpp &= 0x03;

	// Must reset LCD first!
	SH1106_PORT &= ~PIN_RESET;
	delay(1);
	SH1106_PORT |= PIN_RESET;
	
	this->writeLcd(SH1106_COMMAND, 0xAE);    /*display off*/
	this->writeLcd(SH1106_COMMAND, 0x02);    /*set lower column address*/
	this->writeLcd(SH1106_COMMAND, 0x10);    /*set higher column address*/
	this->writeLcd(SH1106_COMMAND, 0x40);    /*set display start line*/
	this->writeLcd(SH1106_COMMAND, 0xB0);    /*set page address*/
	this->writeLcd(SH1106_COMMAND, 0x81);    /*contract control*/
	this->writeLcd(SH1106_COMMAND, contrast);    /*128*/
	this->writeLcd(SH1106_COMMAND, 0xA1);    /*set segment remap*/
	this->writeLcd(SH1106_COMMAND, invertSetting);    /*normal / reverse*/
	this->writeLcd(SH1106_COMMAND, 0xA8);    /*multiplex ratio*/
	this->writeLcd(SH1106_COMMAND, 0x3F);    /*duty = 1/32*/
	this->writeLcd(SH1106_COMMAND, 0xAD);    /*set charge pump enable*/
	this->writeLcd(SH1106_COMMAND, 0x8B);     /*external VCC   */
	this->writeLcd(SH1106_COMMAND, 0x30 | Vpp);    /*0X30---0X33  set VPP   9V liangdu!!!!*/
	this->writeLcd(SH1106_COMMAND, 0xC8);    /*Com scan direction*/
	this->writeLcd(SH1106_COMMAND, 0xD3);    /*set display offset*/
	this->writeLcd(SH1106_COMMAND, 0x00);   /*   0x20  */
	this->writeLcd(SH1106_COMMAND, 0xD5);    /*set osc division*/
	this->writeLcd(SH1106_COMMAND, 0x80);
	this->writeLcd(SH1106_COMMAND, 0xD9);    /*set pre-charge period*/
	this->writeLcd(SH1106_COMMAND, 0x1F);    /*0x22*/
	this->writeLcd(SH1106_COMMAND, 0xDA);    /*set COM pins*/
	this->writeLcd(SH1106_COMMAND, 0x12);
	this->writeLcd(SH1106_COMMAND, 0xDB);    /*set vcomh*/
	this->writeLcd(SH1106_COMMAND, 0x40);
	this->writeLcd(SH1106_COMMAND, 0xAF);    /*display ON*/

	this->clear();
}

size_t SH1106_SPI_FB::write(uint8_t data)
{
	// Non-ASCII characters are not supported.
	if (data < 0x20 || data > 0x7F) return 0;
	
	if (this->m_Position + 5 >= BUF_LEN) this->m_Position -= (BUF_LEN - 6);
	memcpy_P(this->m_Buffer + this->m_Position, ASCII[data - 0x20], 5);
	this->m_Buffer[this->m_Position+5] = 0x00;
	this->m_Position += 6;
	if (this->m_Position >= BUF_LEN) this->m_Position -= BUF_LEN;

	return 1;
}

void SH1106_SPI_FB::clear(bool render)
{
	memset(this->m_Buffer, 0x00, sizeof(this->m_Buffer));
	if (render)
		this->renderAll();
	this->gotoXY(0, 0);
}

uint8_t SH1106_SPI_FB::gotoXY(uint8_t x, uint8_t y) 
{
	if (x >= SH1106_X_PIXELS || y >= SH1106_ROWS) return SH1106_ERROR;
	this->m_Position = (SH1106_X_PIXELS * y) + x;
    x = x + 2;										// Panel is 128 pixels wide, controller RAM has space for 132,
													// it's centered so add an offset to ram address.

    this->writeLcd(SH1106_COMMAND, 0xB0 + y);		// Set row
    this->writeLcd(SH1106_COMMAND, x & 0xF);		// Set lower column address
    this->writeLcd(SH1106_COMMAND, 0x10 | (x >> 4));// Set higher column address
	return SH1106_SUCCESS;
}

uint8_t SH1106_SPI_FB::writeBitmap(const uint8_t *bitmap, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{	
	if (this->gotoXY(x, y) == SH1106_ERROR) return SH1106_ERROR;

	uint8_t *pos = this->m_Buffer + this->m_Position;
    const uint8_t *maxY = bitmap + height * width;	

	for (const uint8_t *y = (uint8_t*) bitmap; y < maxY; y += width)
	{
		memcpy(pos, y, width);
		pos += SH1106_X_PIXELS;
	}
	return SH1106_SUCCESS;
}

void SH1106_SPI_FB::renderAll()
{	
	for (int8_t i = SH1106_ROWS - 1; i >= 0; i--)
	{
		this->gotoXY(0, i);
		this->writeLcd(SH1106_DATA, this->m_Buffer + (i * SH1106_X_PIXELS), SH1106_X_PIXELS);
	}
}

uint8_t SH1106_SPI_FB::renderString(uint8_t x, uint8_t y, uint16_t length)
{
	if (length == 0) return SH1106_ERROR;
	length *= 6;
	length += x;
	for (int8_t i = (length - 1) / SH1106_X_PIXELS; i >= 0; i--, y++)
	{
		if (this->gotoXY(0, y) != SH1106_ERROR)
		{
			this->writeLcd(SH1106_DATA, this->m_Buffer + this->m_Position, SH1106_X_PIXELS);
			this->m_Position += SH1106_X_PIXELS;
		}
	}
	return SH1106_SUCCESS;
}

void SH1106_SPI_FB::setPixel(uint8_t x, uint8_t y, uint8_t value)
{
	if (x >= SH1106_X_PIXELS || y >= SH1106_Y_PIXELS) return;
	uint8_t bank = y / 8;
	uint8_t bitMask = 1 << (y % 8);
	uint8_t &byte = this->m_Buffer[(SH1106_X_PIXELS * bank) + x];
	if (value)
		byte |= bitMask;
	else
		byte &= ~bitMask;
}

uint8_t SH1106_SPI_FB::writeLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	if (x1 == x2 || y1 == y2)
	{
		if (y1 > y2)
			swap(y1, y2);
		if (x1 > x2)
			swap(x1, x2);
		return this->writeRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
	}
	else
		return SH1106_ERROR;
}

void SH1106_SPI_FB::swap(uint8_t &a, uint8_t &b)
{
	uint8_t temp = a;
	a = b;
	b = temp;
}

uint8_t SH1106_SPI_FB::writeRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool fill)
{
	if (x >= SH1106_X_PIXELS || y >= SH1106_Y_PIXELS || width == 0 || height == 0) return SH1106_ERROR;

	// Calculate the bitmasks for the pixels.
	uint8_t bottom = y + height - 1;
	uint8_t bankTop = y / 8;
	uint8_t bankBottom = bottom / 8;
	uint8_t bitMaskTop = 0x01;
	uint8_t bitMaskBottom = 0x80;
	uint8_t bitMaskTopFill = 0xFF;
	uint8_t bitMaskBottomFill = 0xFF;
	bitMaskTop <<= (y % 8);
	bitMaskBottom >>= 7 - (bottom % 8);
	bitMaskTopFill <<= (y % 8);
	bitMaskBottomFill >>= 7 - (bottom % 8);

	// When fill is selected, we'll use the FillMask.
	if (fill)
	{
		bitMaskTop = bitMaskTopFill;
		bitMaskBottom = bitMaskBottomFill;
	}

	// When the rectangle fits in a single bank, we AND the top and bottom masks
	// So that we only fill the required area on the LCD.
	if (bankTop == bankBottom)
	{
		bitMaskTop = fill ? bitMaskTop & bitMaskBottom : bitMaskTop | bitMaskBottom;
		bitMaskTopFill &= bitMaskBottomFill;
	}
	this->gotoXY(x, bankTop);

	// Write the left 'side' of the rectangle on the top bank.
	this->m_Buffer[this->m_Position++] |= bitMaskTopFill;
	// Write a line or a fill.
	for (uint8_t i = 1; i < width-1; i++)
		this->m_Buffer[this->m_Position++] |= bitMaskTop;
	// Write the right 'side' of the rectangle on the top bank.
	if (width > 1)
		this->m_Buffer[this->m_Position++] |= bitMaskTopFill;

	this->m_Position += (SH1106_X_PIXELS - width);

	// Write a fill across the middle banks or two sides of the rectangle.
	if (bankBottom - bankTop > 1)
	{
			for (uint8_t i = bankTop + 1; i < bankBottom; i++)
			{
				if (fill)
					memset(this->m_Buffer + this->m_Position, 0xFF, width);
				else
				{
					this->m_Buffer[this->m_Position] = 0xFF;
					this->m_Buffer[this->m_Position+width-1] = 0xFF;
				}
				this->m_Position += SH1106_X_PIXELS;
			}
	}
	// If the rectangle spans across more than one bank,
	// apply the same logic for the bottom as the top.
	if (bankBottom > bankTop)
	{
		this->m_Buffer[this->m_Position++] |= bitMaskBottomFill;
		for (uint8_t i = 1; i < width-1; i++)
			this->m_Buffer[this->m_Position++] |= bitMaskBottom;
		if (width > 1)
			this->m_Buffer[this->m_Position++] |= bitMaskBottomFill;
	}
	return SH1106_SUCCESS;
}

void SH1106_SPI_FB::writeLcd(uint8_t dataOrCommand, const uint8_t *data, uint16_t count)
{
	SH1106_PORT = (SH1106_PORT & ~PINS_CS_DC) | dataOrCommand;
    for (uint16_t i = count; i > 0; i--)
	{
		SPI.transfer(data[count-i]);
		//SPDR = *data++;
		//while (!(SPSR & _BV(SPIF)));
	}
	SH1106_PORT |= PIN_CS;
}

void SH1106_SPI_FB::writeLcd(uint8_t dataOrCommand, uint8_t data)
{
	SH1106_PORT = (SH1106_PORT & ~PINS_CS_DC) | dataOrCommand;
	SPI.transfer(data);
	//SPDR = data;
	//while (!(SPSR & _BV(SPIF)));
	SH1106_PORT |= PIN_CS;
}
