
/*
 This example code is 16bit mode for TFT01 demo.
 
The demo used TFT01 Mega Shield and connect the pins to Arduino first :
DB0-DB16 to pin 22-29, 30-37
 
All the data pins of Arduino is used ….So , we just can connect the RD to 3.3V ….We don’t used it because we don’t need to read back the data from the screen.
Connect the power pins : LCD-A to 3.3V , VCC to 5V , GND to GND
 
Note that if you haven't TFT01 Mega Shield,the LCM is use the 3.3V voltage level , so if you want to connect the 5V Arduino pins to it , 
you need add a resister about 20K to reduce voltage. We use the 3.3V Arduino Pro which work in 8M , 
so we connect the pins directly — this is why we used 16 bit mode, 8 bit mode is too slow to refresh the screen.
*/
 
#define LCD_RS   38         
#define LCD_WR   39     
#define LCD_CS   40       
#define LCD_REST 41

#define DCLK     6
#define CS       5  
#define DIN      4 
#define DOUT     3
#define IRQ      2 

 
unsigned int TP_X,TP_Y;
void spistar()                                     //SPI Start
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
 
void AD7843(void)              
{
  digitalWrite(CS,LOW);                    
  WriteCharTo7843(0x90); 
  digitalWrite(DCLK,HIGH);
  digitalWrite(DCLK,LOW); 
  TP_Y=ReadFromCharFrom7843();
  WriteCharTo7843(0xD0);   
  digitalWrite(DCLK,HIGH);
  digitalWrite(DCLK,LOW);
  TP_X=ReadFromCharFrom7843();
  digitalWrite(CS,HIGH);
}
 
 
void Lcd_Writ_Bus(char VH,char VL)
{
  PORTA = VH;
  PORTC = VL;
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
 
void Lcd_Write_Com_Data(int com,int dat)
{
     Lcd_Write_Com(com>>8,com);
     Lcd_Write_Data(dat>>8,dat);
}
 
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
    Lcd_Write_Com_Data(0x0003,x1);
  Lcd_Write_Com_Data(0x0006,y1>>8);
  Lcd_Write_Com_Data(0x0007,y1);  
  Lcd_Write_Com_Data(0x0005,x2);
  Lcd_Write_Com_Data(0x0008,y2>>8);
        Lcd_Write_Com_Data(0x0009,y2);
        Lcd_Write_Com(0x00,0x22);                
}
 
void Lcd_Init(void)
{
       digitalWrite(LCD_REST,HIGH);
       delay(5); 
       digitalWrite(LCD_REST,LOW);
       delay(15);
       digitalWrite(LCD_REST,HIGH);
       delay(15);
 
    digitalWrite(LCD_CS,LOW); 
 
        Lcd_Write_Com(0x00,0x83);           
        Lcd_Write_Data(0x00,0x02);  //TESTM=1 
 
        Lcd_Write_Com(0x00,0x85);  
        Lcd_Write_Data(0x00,0x03);  //VDC_SEL=011
        Lcd_Write_Com(0x00,0x8B);  
        Lcd_Write_Data(0x00,0x01);
        Lcd_Write_Com(0x00,0x8C);  
        Lcd_Write_Data(0x00,0x93); //STBA[7]=1,STBA[5:4]=01,STBA[1:0]=11
 
        Lcd_Write_Com(0x00,0x91);  
        Lcd_Write_Data(0x00,0x01); //DCDC_SYNC=1
 
        Lcd_Write_Com(0x00,0x83);  
        Lcd_Write_Data(0x00,0x00); //TESTM=0
         //Gamma Setting
 
        Lcd_Write_Com(0x00,0x3E);  
        Lcd_Write_Data(0x00,0xB0);
        Lcd_Write_Com(0x00,0x3F);  
        Lcd_Write_Data(0x00,0x03);
        Lcd_Write_Com(0x00,0x40);  
        Lcd_Write_Data(0x00,0x10);
        Lcd_Write_Com(0x00,0x41);  
        Lcd_Write_Data(0x00,0x56);
        Lcd_Write_Com(0x00,0x42);  
        Lcd_Write_Data(0x00,0x13);
        Lcd_Write_Com(0x00,0x43);  
        Lcd_Write_Data(0x00,0x46);
        Lcd_Write_Com(0x00,0x44);  
        Lcd_Write_Data(0x00,0x23);
        Lcd_Write_Com(0x00,0x45);  
        Lcd_Write_Data(0x00,0x76);
        Lcd_Write_Com(0x00,0x46);  
        Lcd_Write_Data(0x00,0x00);
        Lcd_Write_Com(0x00,0x47);  
        Lcd_Write_Data(0x00,0x5E);
        Lcd_Write_Com(0x00,0x48);  
        Lcd_Write_Data(0x00,0x4F);
         Lcd_Write_Com(0x00,0x49);  
        Lcd_Write_Data(0x00,0x40);  
//**********Power On sequence************
 
        Lcd_Write_Com(0x00,0x17);  
        Lcd_Write_Data(0x00,0x91);
 
        Lcd_Write_Com(0x00,0x2B);  
        Lcd_Write_Data(0x00,0xF9);
         delay(10);
 
        Lcd_Write_Com(0x00,0x1B);  
        Lcd_Write_Data(0x00,0x14);
 
        Lcd_Write_Com(0x00,0x1A);  
        Lcd_Write_Data(0x00,0x11);
 
        Lcd_Write_Com(0x00,0x1C);  
        Lcd_Write_Data(0x00,0x06);    //0d
 
        Lcd_Write_Com(0x00,0x1F);  
        Lcd_Write_Data(0x00,0x42);
        delay(20);
 
        Lcd_Write_Com(0x00,0x19);  
        Lcd_Write_Data(0x00,0x0A);
 
        Lcd_Write_Com(0x00,0x19);  
        Lcd_Write_Data(0x00,0x1A);
         delay(40);
 
 
        Lcd_Write_Com(0x00,0x19);  
        Lcd_Write_Data(0x00,0x12);
        delay(40);
 
        Lcd_Write_Com(0x00,0x1E);  
        Lcd_Write_Data(0x00,0x27);
        delay(100);    
 
 
       //**********DISPLAY ON SETTING***********
 
        Lcd_Write_Com(0x00,0x24);  
        Lcd_Write_Data(0x00,0x60);
 
        Lcd_Write_Com(0x00,0x3D);  
        Lcd_Write_Data(0x00,0x40);
 
        Lcd_Write_Com(0x00,0x34);  
        Lcd_Write_Data(0x00,0x38);
 
        Lcd_Write_Com(0x00,0x35);  
        Lcd_Write_Data(0x00,0x38);
 
        Lcd_Write_Com(0x00,0x24);  
        Lcd_Write_Data(0x00,0x38);
       delay(40);
 
        Lcd_Write_Com(0x00,0x24);  
        Lcd_Write_Data(0x00,0x3C);
 
        Lcd_Write_Com(0x00,0x16);  
        Lcd_Write_Data(0x00,0x1C);
 
        Lcd_Write_Com(0x00,0x01);  
        Lcd_Write_Data(0x00,0x06);
 
        Lcd_Write_Com(0x00,0x55);  
        Lcd_Write_Data(0x00,0x00); 
 
        Lcd_Write_Com(0x00,0x02);           
        Lcd_Write_Data(0x00,0x00);
        Lcd_Write_Com(0x00,0x03);           
        Lcd_Write_Data(0x00,0x00);
          Lcd_Write_Com(0x00,0x04);           
        Lcd_Write_Data(0x00,0x00);
        Lcd_Write_Com(0x00,0x05);           
        Lcd_Write_Data(0x00,0xef);
 
        Lcd_Write_Com(0x00,0x06);           
        Lcd_Write_Data(0x00,0x00);
        Lcd_Write_Com(0x00,0x07);           
        Lcd_Write_Data(0x00,0x00);
        Lcd_Write_Com(0x00,0x08);           
        Lcd_Write_Data(0x00,0x01);
        Lcd_Write_Com(0x00,0x09);           
        Lcd_Write_Data(0x00,0x8f);
 
        Lcd_Write_Com(0x00,0x22);
 
    digitalWrite(LCD_CS,HIGH); 
}
 
void Pant(char VH,char VL)
{
  int i,j;
  digitalWrite(LCD_CS,LOW); 
  Address_set(0,0,239,399);
  for(i=0;i<400;i++)
  {
    for (j=0;j<240;j++)
    {
      Lcd_Write_Data(VH,VL);
    }
 
  }
  digitalWrite(LCD_CS,HIGH);  
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
  Pant(0x00,0x00);   
}
 
void loop()
{ 
  unsigned char flag;
  unsigned char ss[6];  
  unsigned int lx,ly;
  spistar();  
  int pacy=random(0, 7);  
  while(digitalRead(IRQ)==0)
  {  
      digitalWrite(LCD_CS,LOW); 
      AD7843();
      lx=(TP_X- 310)/14;
      ly=(TP_Y- 150)/9;
      Address_set(lx,ly,lx+2,ly+2);
      switch(pacy)
                                {
                                  case 0: for(int i=0; i<5; i++)  Lcd_Write_Data(0xF8,0x00);  break;   //Red
                                  case 1: for(int i=0; i<5; i++)  Lcd_Write_Data(0xFF,0xE0);  break;   //Yellow
                                  case 2: for(int i=0; i<5; i++)  Lcd_Write_Data(0xFF,0xFF);  break;   //White 
                                  case 3: for(int i=0; i<5; i++)  Lcd_Write_Data(0x05,0x1F);  break;   //Blue
                                  case 4: for(int i=0; i<5; i++)  Lcd_Write_Data(0x00,0x1F);  break;   //Blue-2
                                  case 5: for(int i=0; i<5; i++)  Lcd_Write_Data(0xF8,0x1F);  break;   //Magenta
                                  case 6: for(int i=0; i<5; i++)  Lcd_Write_Data(0x07,0xE0);  break;   //Green
                                  case 7: for(int i=0; i<5; i++)  Lcd_Write_Data(0x7F,0xFF);  break;   //Cyan
                                  defoult:for(int i=0; i<5; i++)  Lcd_Write_Data(0x00,0x00);  break;   //Black
                                }
    }
    digitalWrite(LCD_CS,HIGH); 
}
