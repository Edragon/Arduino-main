case HX8357B:
	LCD_Write_COM(0x11);//Sleep Out  
	delay(120);  
	
	LCD_Write_COM(0xEE);//Set EQ  
	LCD_Write_DATA(0x02);  
	LCD_Write_DATA(0x01);  
	LCD_Write_DATA(0x02);  
	LCD_Write_DATA(0x01);  
	
	LCD_Write_COM(0xED);//Set DIR TIM  
	LCD_Write_DATA(0x00);  
	LCD_Write_DATA(0x00);  
	LCD_Write_DATA(0x9A);  
	LCD_Write_DATA(0x9A);  
	LCD_Write_DATA(0x9B);  
	LCD_Write_DATA(0x9B);  
	LCD_Write_DATA(0x00);  
	LCD_Write_DATA(0x00);  
	LCD_Write_DATA(0x00);  
	LCD_Write_DATA(0x00);  
	LCD_Write_DATA(0xAE);  
	LCD_Write_DATA(0xAE);  
	LCD_Write_DATA(0x01);  
	LCD_Write_DATA(0xA2);  
	LCD_Write_DATA(0x00);  
	
	LCD_Write_COM(0xB4);//Set RM, DM  
	LCD_Write_DATA(0x00);//  
	
	LCD_Write_COM(0xC0); //Set Panel Driving
	LCD_Write_DATA(0x10); //REV SM GS
	LCD_Write_DATA(0x3B); // NL[5:0]  
	LCD_Write_DATA(0x00); //SCN[6:0]  
	LCD_Write_DATA(0x02); //NDL 0 PTS[2:0]
	LCD_Write_DATA(0x11); //PTG ISC[3:0]  
	
	LCD_Write_COM(0xC1);//
	LCD_Write_DATA(0x10);//line inversion
	
	LCD_Write_COM(0xC8);//Set Gamma  
	LCD_Write_DATA(0x00); //KP1,KP0
	LCD_Write_DATA(0x46); //KP3,KP2
	LCD_Write_DATA(0x12); //KP5,KP4
	LCD_Write_DATA(0x20); //RP1,RP0
	LCD_Write_DATA(0x0c); //VRP0 01
	LCD_Write_DATA(0x00); //VRP1
	LCD_Write_DATA(0x56); //KN1,KN0
	LCD_Write_DATA(0x12); //KN3,KN2
	LCD_Write_DATA(0x67); //KN5,KN4
	LCD_Write_DATA(0x02); //RN1,RN0
	LCD_Write_DATA(0x00); //VRN0
	LCD_Write_DATA(0x0c); //VRN1 01
	
	LCD_Write_COM(0xD0);//Set Power  
	LCD_Write_DATA(0x44);//DDVDH :5.28
	LCD_Write_DATA(0x42); // BT VGH:15.84 VGL:-7.92
	LCD_Write_DATA(0x06);//VREG1 4.625V
	
	LCD_Write_COM(0xD1);//Set VCOM  
	LCD_Write_DATA(0x73); //VCOMH
	LCD_Write_DATA(0x16);
	
	LCD_Write_COM(0xD2);  
	LCD_Write_DATA(0x04);  
	LCD_Write_DATA(0x22); //12
	
	LCD_Write_COM(0xD3);  
	LCD_Write_DATA(0x04);  
	LCD_Write_DATA(0x12);  
	
	LCD_Write_COM(0xD4);  
	LCD_Write_DATA(0x07);  
	LCD_Write_DATA(0x12);  
	
	LCD_Write_COM(0xE9); //Set Panel
	LCD_Write_DATA(0x00);
	
	LCD_Write_COM(0xC5); //Set Frame rate
	LCD_Write_DATA(0x08); //61.51Hz
	
	LCD_Write_COM(0X0036);
	LCD_Write_DATA(0X000a); //竖屏参数
	LCD_Write_DATA(0x003B); //横屏参数
	LCD_Write_COM(0X003A);
	LCD_Write_DATA(0X0055);
	
	LCD_Write_COM(0X002A);
	LCD_Write_DATA(0X0000);
	LCD_Write_DATA(0X0000);
	LCD_Write_DATA(0X0001);
	LCD_Write_DATA(0X003F);
	
	LCD_Write_COM(0X002B);
	LCD_Write_DATA(0X0000);
	LCD_Write_DATA(0X0000);
	LCD_Write_DATA(0X0001);
	LCD_Write_DATA(0X00E0);
	delay(120);
	
	
	LCD_Write_COM(0x35);
	LCD_Write_DATA(0x00);//TE ON
	
	LCD_Write_COM(0x29); //Display On
	delay(5);

	break;
