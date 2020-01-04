/***************************************************************************************************************************************************************
This example code is 16bit mode for TFT4.3 demo.
 
The demo used TFT01 Mega Shield and connect the pins to Arduino first :
DB0-DB16 to pin 22-29, 30-37
 
All the data pins of Arduino is used ….So , we just can connect the RD to 3.3V ….We don’t used it because we don’t need to read back the data from the screen.
Connect the power pins : LCD-A to 3.3V , VCC to 5V , GND to GND
 
Note that if you haven't TFT01 Mega Shilld,the LCM is use the 3.3V voltage level , so if you want to connect the 5V Arduino pins to it , 
you need add a resister about 20K to reduce voltage. We use the 3.3V Arduino Pro which work in 8M , 
so we connect the pins directly — this is why we used 16 bit mode, 8 bit mode is too slow to refresh the screen.

Create by Robi.Wang  On 9-June-2012 
Version 2.2  www.elecfreaks.com
***************************************************************************************************************************************************************/

/* Display IO */ 
#define LCD_RS   38         
#define LCD_WR   39     
#define LCD_CS   40       
#define LCD_REST 41

/* Touch IO  */
#define DCLK     6
#define CS       5  
#define DIN      4 
#define DOUT     3
#define IRQ      2 

unsigned int  HDP=479;
unsigned int  HT=531;
unsigned int  HPS=43;
unsigned int  LPS=8;
unsigned char HPW=10;

unsigned int  VDP=271;
unsigned int  VT=288;
unsigned int  VPS=12;
unsigned int  FPS=4;
unsigned char VPW=10;
;

struct pix_
{
	unsigned int x;
	unsigned int y;
};
struct pix_ Tp_pix;	 //当前触控坐标的取样值  

void spistar()           //SPI Start
{
  digitalWrite(CS,HIGH);
  digitalWrite(DCLK,HIGH);
  digitalWrite(DIN,HIGH);
  digitalWrite(DCLK,HIGH);
 
}
//**********************************************************
void WriteCharTo7843(unsigned char num)          //SPI Write Data
{
  unsigned char count=0;
  unsigned char temp;
  unsigned nop;
  temp=num;
  digitalWrite(DCLK,LOW);
  for(count=0;count<8;count++)
  {
    if(temp&0x80)
      digitalWrite(DIN,HIGH);
    else
      digitalWrite(DIN,LOW);
 
    temp=temp<<1; 
 
    digitalWrite(DCLK,LOW);                
    nop++;
    nop++;
    digitalWrite(DCLK,HIGH);
    nop++;
    nop++;
  }
}
 
//**********************************************************
unsigned int ReadFromCharFrom7843()             //SPI Read Data
{ 
  unsigned nop;
  unsigned char count=0;
  unsigned int Num=0;
  for(count=0;count<12;count++)
  {
    Num<<=1;
    digitalWrite(DCLK,HIGH);//DCLK=1; _nop_();_nop_();_nop_();                
    nop++;
    digitalWrite(DCLK,LOW);//DCLK=0; _nop_();_nop_();_nop_();
    nop++;
    if(digitalRead(DOUT)) Num++;
  }
  return(Num);
}
 
//void AD7843(void) 
struct pix_ AD7843()  
{
  struct pix_ pix;
  digitalWrite(CS,LOW);                    
  WriteCharTo7843(0x90); 
  digitalWrite(DCLK,HIGH);
  digitalWrite(DCLK,LOW); 
  pix.y=ReadFromCharFrom7843();
  WriteCharTo7843(0xD0);   
  digitalWrite(DCLK,HIGH);
  digitalWrite(DCLK,LOW);
  pix.x=ReadFromCharFrom7843();
  digitalWrite(CS,HIGH);
  return pix;
}
 
void Lcd_Writ_Bus(char VH,char VL)
{
  unsigned char i,temp,data; 
  data=VH;
  for(i=22;i<=29;i++)
  {
    temp=(data&0x01);
    if(temp)
      digitalWrite(i,HIGH);
    else
      digitalWrite(i,LOW);
    data=data>>1;
  }	 
  data=VL; 
  for(i=37;i>=30;i--)
  {
    temp=(data&0x01);
    if(temp)
      digitalWrite(i,HIGH);
    else
      digitalWrite(i,LOW);
    data=data>>1;
  }	
  digitalWrite(LCD_WR,LOW);
  digitalWrite(LCD_WR,HIGH);
}
 
void Lcd_Write_Com(char VH,char VL)  
{   
  digitalWrite(LCD_RS,LOW);
  Lcd_Writ_Bus(VH,VL);
}
 
void Lcd_Write_Data(char VH,char VL)
{
  digitalWrite(LCD_RS,HIGH);
  Lcd_Writ_Bus(VH,VL);
}

void LCD_WR_Data(int da)
{
    digitalWrite(LCD_RS, 1);
    Lcd_Writ_Bus(da>>8,da);
}	
 
void LCD_WR_REG(int da)	 
{	
    digitalWrite(LCD_RS,0);
    Lcd_Writ_Bus(da>>8,da);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
    LCD_WR_REG(0x002A);	
    LCD_WR_Data(x1>>8);	    
    LCD_WR_Data(x1&0x00ff);
    LCD_WR_Data(x2>>8);	    
    LCD_WR_Data(x2&0x00ff);
    LCD_WR_REG(0x002b);	
    LCD_WR_Data(y1>>8);	    
    LCD_WR_Data(y1&0x00ff);
    LCD_WR_Data(y2>>8);	    
    LCD_WR_Data(y2&0x00ff);
    LCD_WR_REG(0x002c);							 
}
 
void Lcd_Init(void)
{
       digitalWrite(LCD_REST,HIGH);
       delay(5); 
       digitalWrite(LCD_REST,LOW);
       delay(15);
       digitalWrite(LCD_REST,HIGH);
       delay(15);
       digitalWrite(LCD_CS ,HIGH);
       digitalWrite(LCD_WR ,HIGH);
       digitalWrite(LCD_CS, LOW);
 
        LCD_WR_REG(0x00E2);	//PLL multiplier, set PLL clock to 120M
	LCD_WR_Data(0x002d);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	LCD_WR_Data(0x0002);
	LCD_WR_Data(0x0004);
	LCD_WR_REG(0x00E0);  // PLL enable
	LCD_WR_Data(0x0001);
	delay(1);
	LCD_WR_REG(0x00E0);
	LCD_WR_Data(0x0003);
	delay(5);
	LCD_WR_REG(0x0001);  // software reset
	delay(5);
	LCD_WR_REG(0x00E6);	//PLL setting for PCLK, depends on resolution
	LCD_WR_Data(0x0000);
	LCD_WR_Data(0x00ff);
	LCD_WR_Data(0x00be);

	LCD_WR_REG(0x00B0);	//LCD SPECIFICATION
	LCD_WR_Data(0x0020);
	LCD_WR_Data(0x0000);
	LCD_WR_Data((HDP>>8)&0X00FF);  //Set HDP
	LCD_WR_Data(HDP&0X00FF);
        LCD_WR_Data((VDP>>8)&0X00FF);  //Set VDP
	LCD_WR_Data(VDP&0X00FF);
        LCD_WR_Data(0x0000);
	delay(5);
	LCD_WR_REG(0x00B4);	//HSYNC
	LCD_WR_Data((HT>>8)&0X00FF);  //Set HT
	LCD_WR_Data(HT&0X00FF);
	LCD_WR_Data((HPS>>8)&0X00FF);  //Set HPS
	LCD_WR_Data(HPS&0X00FF);
	LCD_WR_Data(HPW);			   //Set HPW
	LCD_WR_Data((LPS>>8)&0X00FF);  //SetLPS
	LCD_WR_Data(LPS&0X00FF);
	LCD_WR_Data(0x0000);

	LCD_WR_REG(0x00B6);	//VSYNC
	LCD_WR_Data((VT>>8)&0X00FF);   //Set VT
	LCD_WR_Data(VT&0X00FF);
	LCD_WR_Data((VPS>>8)&0X00FF);  //Set VPS
	LCD_WR_Data(VPS&0X00FF);
	LCD_WR_Data(VPW);			   //Set VPW
	LCD_WR_Data((FPS>>8)&0X00FF);  //Set FPS
	LCD_WR_Data(FPS&0X00FF);

	LCD_WR_REG(0x0036); //rotation
	LCD_WR_Data(0x0000);

	LCD_WR_REG(0x00F0); //pixel data interface
	LCD_WR_Data(0x0003);

	delay(5);
	LCD_WR_REG(0x0029); //display on

	LCD_WR_REG(0x00BE); //set PWM for B/L
	LCD_WR_Data(0x0006);
	LCD_WR_Data(0x00f0);
	LCD_WR_Data(0x0001);
	LCD_WR_Data(0x00f0);
	LCD_WR_Data(0x0000);
	LCD_WR_Data(0x0000);

	LCD_WR_REG(0x00d0); 
	LCD_WR_Data(0x000d);

        //----------LCD RESET---GPIO0-------------------//
	LCD_WR_REG(0x00B8);
	LCD_WR_Data(0x0000);    //GPIO3=input, GPIO[2:0]=output
	LCD_WR_Data(0x0001);    //GPIO0 normal

	LCD_WR_REG(0x00BA);
	LCD_WR_Data(0x0000);  
}
 
void Pant(char VH,char VL)
{
  int i,j;
  Address_set(0,0,479,271);
  for(i=0;i<480;i++)
  {
    for (j=0;j<272;j++)
    {
      Lcd_Write_Data(VH,VL);
    }
  }
}
 
unsigned char Makpix(struct pix_ pix1,struct pix_ pix2) //Filtering
{
    unsigned char l=0;
    int x=pix1.x>pix2.x?pix1.x-pix2.x:pix2.x-pix1.x;
    int y=pix1.y>pix2.y?pix1.y-pix2.y:pix2.y-pix1.y;
    if(x<10&&y<10)	
    {
    	l=1;
    	Tp_pix.x=pix1.x;
    	Tp_pix.y=pix1.y;
    }
    return l;
}

unsigned char Getpix() 
{
	struct pix_ pix1;
	struct pix_ pix2; 
	pix1=AD7843();
	pix2=AD7843();
 	return	Makpix(pix1,pix2);	
}

void setup()
{
  for(int p=22;p<42;p++)
  {
    pinMode(p,OUTPUT);
  }
  for(int p=2; p<7;p++)
      pinMode(p,OUTPUT);
  pinMode(DOUT,INPUT);
  pinMode(IRQ,INPUT);
  
  Lcd_Init();     
  Pant(0xff, 0xff);  
  Pant(0x00,0x00);  
  delay(500);
  spistar();  
}
 
void loop(){
                unsigned int lx,ly;
                int pacy=random(0, 7);
		while(digitalRead(IRQ)==0)
		{
			if(Getpix()==1)
			{
				lx=480-((Tp_pix.x-380)*10/69);
				if(lx>479) lx=479;
				ly=272-((Tp_pix.y-600)*10/111);
                                if(ly>270) ly=270;
				Address_set(lx,ly,lx+2,ly+2);
                                
                                switch(pacy)
                                {
                                  case 0: for(int i=0; i<7; i++)  Lcd_Write_Data(0xF8,0x00);  break;   //Red
                                  case 1: for(int i=0; i<7; i++)  Lcd_Write_Data(0xFF,0xE0);  break;   //Yellow
                                  case 2: for(int i=0; i<7; i++)  Lcd_Write_Data(0xFF,0xFF);  break;   //White 
                                  case 3: for(int i=0; i<7; i++)  Lcd_Write_Data(0x05,0x1F);  break;   //Blue
                                  case 4: for(int i=0; i<7; i++)  Lcd_Write_Data(0x00,0x1F);  break;   //Blue-2
                                  case 5: for(int i=0; i<7; i++)  Lcd_Write_Data(0xF8,0x1F);  break;   //Magenta
                                  case 6: for(int i=0; i<7; i++)  Lcd_Write_Data(0x07,0xE0);  break;   //Green
                                  case 7: for(int i=0; i<7; i++)  Lcd_Write_Data(0x7F,0xFF);  break;   //Cyan
                                  defoult:for(int i=0; i<7; i++)  Lcd_Write_Data(0x00,0x00);  break;   //Black
                                }
	
		   	}
		}	
}
