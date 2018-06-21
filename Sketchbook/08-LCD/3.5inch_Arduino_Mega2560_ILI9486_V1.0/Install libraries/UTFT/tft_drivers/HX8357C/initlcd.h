case HX8357C:
//for Truly 3.0" LCD Panel
	LCD_Write_COM(0XB9);//Set EXTC
	LCD_Write_DATA(0xFF);
	LCD_Write_DATA(0x83);
	LCD_Write_DATA(0x57);
	delay(50);
	
	LCD_Write_COM(0XB6);//Set VCOMDC
	LCD_Write_DATA(0x2C);//0x52 for HSD 3.0"
	
	LCD_Write_COM(0X11);//
	delay(200);
	
	LCD_Write_COM(0X35);//TE ON
	LCD_Write_COM(0X003A);
	LCD_Write_DATA(0X0055);//65K

	/*
	LCD_Write_COM(0XCC);//Set Panel
	LCD_Write_DATA(0X09);//
	*/
	/*
	LCD_Write_COM(0XB3);//COLOR FORMAT
	LCD_Write_DATA(0X43);//
	LCD_Write_DATA(0X00);//
	LCD_Write_DATA(0X06);//
	LCD_Write_DATA(0X06);//
	*/
	LCD_Write_COM(0XB1);//
	LCD_Write_DATA(0X00);//
	LCD_Write_DATA(0X15);//
	LCD_Write_DATA(0X0D);//0D
	LCD_Write_DATA(0X0D);//0D
	LCD_Write_DATA(0X83);//
	LCD_Write_DATA(0X48);//
	
	
	LCD_Write_COM(0XC0);//
	LCD_Write_DATA(0X24);//
	LCD_Write_DATA(0X24);//
	LCD_Write_DATA(0X01);//
	LCD_Write_DATA(0X3C);//
	LCD_Write_DATA(0XC8);//
	LCD_Write_DATA(0X08);//
	
	LCD_Write_COM(0XB4);//
	LCD_Write_DATA(0X02);//
	LCD_Write_DATA(0X40);//
	LCD_Write_DATA(0X00);//
	LCD_Write_DATA(0X2A);//
	LCD_Write_DATA(0X2A);//
	LCD_Write_DATA(0X0D);//
	LCD_Write_DATA(0X4F);//
	
	LCD_Write_COM(0XE0);//
	LCD_Write_DATA(0X00);//
	LCD_Write_DATA(0X15);//
	LCD_Write_DATA(0X1D);//
	LCD_Write_DATA(0X2A);//
	LCD_Write_DATA(0X31);//
	LCD_Write_DATA(0X42);//
	LCD_Write_DATA(0X4C);//
	LCD_Write_DATA(0X53);//
	LCD_Write_DATA(0X45);//
	LCD_Write_DATA(0X40);//
	LCD_Write_DATA(0X3B);//
	LCD_Write_DATA(0X32);//
	LCD_Write_DATA(0X2E);//
	LCD_Write_DATA(0X28);//
	
	LCD_Write_DATA(0X24);//
	LCD_Write_DATA(0X03);//
	LCD_Write_DATA(0x00);//
	LCD_Write_DATA(0X15);//
	LCD_Write_DATA(0X1D);//
	LCD_Write_DATA(0X2A);//
	LCD_Write_DATA(0X31);//
	LCD_Write_DATA(0X42);//
	LCD_Write_DATA(0X4C);//
	LCD_Write_DATA(0X53);//
	LCD_Write_DATA(0X45);//
	LCD_Write_DATA(0X40);//
	LCD_Write_DATA(0X3B);//
	LCD_Write_DATA(0X32);//
	
	LCD_Write_DATA(0X2E);//
	LCD_Write_DATA(0X28);//
	LCD_Write_DATA(0X24);//
	LCD_Write_DATA(0X03);//
	LCD_Write_DATA(0X00);//
	LCD_Write_DATA(0X01);//

	LCD_Write_COM(0x36);
	LCD_Write_DATA(0x48);
	delay(20);
	LCD_Write_COM(0x21);//Display inversion ON
	delay(20);
	LCD_Write_COM(0x29);
	
	delay(120);

	break;
