case ILI9225:
	LCD_Write_COM_DATA(0x01,0x011c);
	LCD_Write_COM_DATA(0x02,0x0100);
	LCD_Write_COM_DATA(0x03,0x1038);
	LCD_Write_COM_DATA(0x08,0x0808); // set BP and FP
	LCD_Write_COM_DATA(0x0B,0x1100); //frame cycle
	LCD_Write_COM_DATA(0x0C,0x0000); // RGB interface setting R0Ch=0x0110 for RGB 18Bit and R0Ch=0111for RGB16Bit
	LCD_Write_COM_DATA(0x0F,0x1401); // Set frame rate----0801
	LCD_Write_COM_DATA(0x15,0x0000); //set system interface
	LCD_Write_COM_DATA(0x20,0x0000); // Set GRAM Address
	LCD_Write_COM_DATA(0x21,0x0000); // Set GRAM Address
	//*************Power On sequence ****************//
	delay(50);                         // Delay 50ms
	LCD_Write_COM_DATA(0x10,0x0800); // Set SAP,DSTB,STB----0A00
	LCD_Write_COM_DATA(0x11,0x1F3F); // Set APON,PON,AON,VCI1EN,VC----1038
	delay(50);                         // Delay 50ms
	LCD_Write_COM_DATA(0x12,0x0121); // Internal reference voltage= Vci;----1121
	LCD_Write_COM_DATA(0x13,0x006F); // Set GVDD----0066
	LCD_Write_COM_DATA(0x14,0x4349); // Set VCOMH/VCOML voltage----5F60
	//-------------- Set GRAM area -----------------//
	LCD_Write_COM_DATA(0x30,0x0000);
	LCD_Write_COM_DATA(0x31,0x00DB);
	LCD_Write_COM_DATA(0x32,0x0000);
	LCD_Write_COM_DATA(0x33,0x0000);
	LCD_Write_COM_DATA(0x34,0x00DB);
	LCD_Write_COM_DATA(0x35,0x0000);
	LCD_Write_COM_DATA(0x36,0x00AF);
	LCD_Write_COM_DATA(0x37,0x0000);
	LCD_Write_COM_DATA(0x38,0x00DB);
	LCD_Write_COM_DATA(0x39,0x0000);
	// ----------- Adjust the Gamma Curve ----------//
	LCD_Write_COM_DATA(0x50,0x0001);  //0400
	LCD_Write_COM_DATA(0x51,0x200B);  //060B
	LCD_Write_COM_DATA(0x52,0x0000);  //0C0A
	LCD_Write_COM_DATA(0x53,0x0404);  //0105
	LCD_Write_COM_DATA(0x54,0x0C0C);  //0A0C
	LCD_Write_COM_DATA(0x55,0x000C);  //0B06
	LCD_Write_COM_DATA(0x56,0x0101);  //0004
	LCD_Write_COM_DATA(0x57,0x0400);  //0501
	LCD_Write_COM_DATA(0x58,0x1108);  //0E00
	LCD_Write_COM_DATA(0x59,0x050C);  //000E
	delay(50);                                     // Delay 50ms
	LCD_Write_COM_DATA(0x07,0x1017);
	LCD_Write_COM(0x22);
break;
