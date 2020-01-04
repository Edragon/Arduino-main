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
      Lcd_Write_Com_Data(0x0044,(x2<<8)+x1);
      Lcd_Write_Com_Data(0x0045,y1);
      Lcd_Write_Com_Data(0x0046,y2);
      Lcd_Write_Com_Data(0x004e,x1);
      Lcd_Write_Com_Data(0x004f,y1);
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
 
    Lcd_Write_Com_Data(0x0000,0x0001);    delay(1);  //打开晶振
    Lcd_Write_Com_Data(0x0003,0xA8A4);    delay(1);   //0xA8A4
    Lcd_Write_Com_Data(0x000C,0x0000);    delay(1);   
    Lcd_Write_Com_Data(0x000D,0x080C);    delay(1);   
    Lcd_Write_Com_Data(0x000E,0x2B00);    delay(1);   
    Lcd_Write_Com_Data(0x001E,0x00B7);    delay(1);   
    Lcd_Write_Com_Data(0x0001,0x2B3F);    delay(1);   //驱动输出控制320*240  0x6B3F
    Lcd_Write_Com_Data(0x0002,0x0600);    delay(1);
    Lcd_Write_Com_Data(0x0010,0x0000);    delay(1);
    Lcd_Write_Com_Data(0x0011,0x6070);    delay(1);        //0x4030           //定义数据格式  16位色 
    Lcd_Write_Com_Data(0x0005,0x0000);    delay(1);
    Lcd_Write_Com_Data(0x0006,0x0000);    delay(1);
    Lcd_Write_Com_Data(0x0016,0xEF1C);    delay(1);
    Lcd_Write_Com_Data(0x0017,0x0003);    delay(1);
    Lcd_Write_Com_Data(0x0007,0x0233);    delay(1);        //0x0233       
    Lcd_Write_Com_Data(0x000B,0x0000);    delay(1);
    Lcd_Write_Com_Data(0x000F,0x0000);    delay(1);        //扫描开始地址
    Lcd_Write_Com_Data(0x0041,0x0000);    delay(1);
    Lcd_Write_Com_Data(0x0042,0x0000);    delay(1);
    Lcd_Write_Com_Data(0x0048,0x0000);    delay(1);
    Lcd_Write_Com_Data(0x0049,0x013F);    delay(1);
    Lcd_Write_Com_Data(0x004A,0x0000);    delay(1);
    Lcd_Write_Com_Data(0x004B,0x0000);    delay(1);
    Lcd_Write_Com_Data(0x0044,0xEF00);    delay(1);
    Lcd_Write_Com_Data(0x0045,0x0000);    delay(1);
    Lcd_Write_Com_Data(0x0046,0x013F);    delay(1);
    Lcd_Write_Com_Data(0x0030,0x0707);    delay(1);
    Lcd_Write_Com_Data(0x0031,0x0204);    delay(1);
    Lcd_Write_Com_Data(0x0032,0x0204);    delay(1);
    Lcd_Write_Com_Data(0x0033,0x0502);    delay(1);
    Lcd_Write_Com_Data(0x0034,0x0507);    delay(1);
    Lcd_Write_Com_Data(0x0035,0x0204);    delay(1);
    Lcd_Write_Com_Data(0x0036,0x0204);    delay(1);
    Lcd_Write_Com_Data(0x0037,0x0502);    delay(1);
    Lcd_Write_Com_Data(0x003A,0x0302);    delay(1);
    Lcd_Write_Com_Data(0x003B,0x0302);    delay(1);
    Lcd_Write_Com_Data(0x0023,0x0000);    delay(1);
    Lcd_Write_Com_Data(0x0024,0x0000);    delay(1);
    Lcd_Write_Com_Data(0x0025,0x8000);    delay(1);
    Lcd_Write_Com_Data(0x004f,0);        //行首址0
    Lcd_Write_Com_Data(0x004e,0);        //列首址0
    Lcd_Write_Com(0x00,0x22); 
 
}
 
void Pant(char VH,char VL)
{
  int i,j;
  digitalWrite(LCD_CS,LOW); 
  Address_set(0,0,239,319);
  for(i=0;i<320;i++)
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
  Pant(0x00, 0x00);
  delay(500); 
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
      lx=((TP_X-340)*10/144);
      if(lx>237) lx=237;
      ly=320-((TP_Y-320)/11);
      if(ly<0) ly=0;
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
      digitalWrite(LCD_CS,HIGH); 
    }
}

