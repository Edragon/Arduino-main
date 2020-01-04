case SPFD5420:
	LCD_Write_COM(0x210);	
	LCD_Write_DATA(x1);
	LCD_Write_COM(0x211);		
	LCD_Write_DATA(x2);
	LCD_Write_COM(0x212);	
	LCD_Write_DATA(y1);
	LCD_Write_COM(0x213);		
	LCD_Write_DATA(y2);
	
	LCD_Write_COM(0x200);	
	LCD_Write_DATA(x1);
	LCD_Write_COM(0x201);	
	LCD_Write_DATA(y1);
  LCD_Write_COM(0x202); 							 
	break;
