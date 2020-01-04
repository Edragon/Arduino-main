/*
 This example code is 16bit mode for TFT01 demo.
 
Connect the pins to Arduino first :
DB0-DB16 to pin D0-D13 , pin A0-A1 of Arduino
RESET to A2
CS to A3
WR to A4
RS to A5
 
All the data pins of Arduino is used ….So , we just can connect the RD to 3.3V.
We don’t used it because we don’t need to read back the data from the screen.
Connect the power pins : LCD-A to 3.3V , VCC to 5V , GND to GND
 
Note that the LCD is use the 3.3V voltage level , so if you want to connect the 5V Arduino pins to it , 
you need add a resister about 20K to reduce voltage. We use the 3.3V Arduino Pro which work in 8M , 
so we connect the pins directly — this is why we used 16 bit mode, 8 bit mode is too slow to refresh the screen.
 
*/
 
#define Bus_16   
#define LCD_RS   19         
#define LCD_WR   18     
#define LCD_CS   17       
#define LCD_REST 16

void Lcd_Writ_Bus(char VH,char VL)
{
  unsigned char i,temp,data; 
  data=VH;
  for(i=0;i<8;i++)
  {
    temp=(data&0x01);
    if(temp)
      digitalWrite(i,HIGH);
    else
      digitalWrite(i,LOW);
    data=data>>1;
  }   
  data=VL; 
  for(i=8;i<16;i++)
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
 
void Lcd_Write_Com_Data(int com,int dat)
{
     Lcd_Write_Com(com>>8,com);
     Lcd_Write_Data(dat>>8,dat);
}
 
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
      Lcd_Write_Com(0x00,0x20);
      Lcd_Write_Data(x1>>8,x1);   //setting X
      Lcd_Write_Com(0x00,0x21);
      Lcd_Write_Data(y1>>8,y1);   //setting Y
      Lcd_Write_Com(0x00,0x50);
      Lcd_Write_Data(x1>>8,x1);   //starting X
      Lcd_Write_Com(0x00,0x52);
      Lcd_Write_Data(y1>>8,y1);   //starting Y
      Lcd_Write_Com(0x00,0x51);
      Lcd_Write_Data(x2>>8,x2);   //ending X
      Lcd_Write_Com(0x00,0x53);
      Lcd_Write_Data(y2>>8,y2);   //ending Y
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
        Lcd_Write_Com_Data(0x0001, 0x0100); // set SS and SM bit
  Lcd_Write_Com_Data(0x0002, 0x0200); // set 1 line inversion
  Lcd_Write_Com_Data(0x0003, 0x1030); // set GRAM write direction and BGR=1.
  Lcd_Write_Com_Data(0x0004, 0x0000); // Resize register
  Lcd_Write_Com_Data(0x0008, 0x0207); // set the back porch and front porch
  Lcd_Write_Com_Data(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
  Lcd_Write_Com_Data(0x000A, 0x0000); // FMARK function
  Lcd_Write_Com_Data(0x000C, 0x0000); // RGB interface setting
  Lcd_Write_Com_Data(0x000D, 0x0000); // Frame marker Position
  Lcd_Write_Com_Data(0x000F, 0x0000); // RGB interface polarity
  //*************Power On sequence ****************//
  Lcd_Write_Com_Data(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
  Lcd_Write_Com_Data(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
  Lcd_Write_Com_Data(0x0012, 0x0000); // VREG1OUT voltage
  Lcd_Write_Com_Data(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
  Lcd_Write_Com_Data(0x0007, 0x0001);
  delay(200); // Dis-charge capacitor power voltage
  Lcd_Write_Com_Data(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
  Lcd_Write_Com_Data(0x0011, 0x0227); // Set DC1[2:0], DC0[2:0], VC[2:0]
  delay(50); // Delay 50ms
  Lcd_Write_Com_Data(0x0012, 0x000D); // 0012
  delay(50); // Delay 50ms
  Lcd_Write_Com_Data(0x0013, 0x1200); // VDV[4:0] for VCOM amplitude
  Lcd_Write_Com_Data(0x0029, 0x000A); // 04  VCM[5:0] for VCOMH
  Lcd_Write_Com_Data(0x002B, 0x000D); // Set Frame Rate
  delay(50); // Delay 50ms
  Lcd_Write_Com_Data(0x0020, 0x0000); // GRAM horizontal Address
  Lcd_Write_Com_Data(0x0021, 0x0000); // GRAM Vertical Address
  // ----------- Adjust the Gamma Curve ----------//
  Lcd_Write_Com_Data(0x0030, 0x0000);
  Lcd_Write_Com_Data(0x0031, 0x0404);
  Lcd_Write_Com_Data(0x0032, 0x0003);
  Lcd_Write_Com_Data(0x0035, 0x0405);
  Lcd_Write_Com_Data(0x0036, 0x0808);
  Lcd_Write_Com_Data(0x0037, 0x0407);
  Lcd_Write_Com_Data(0x0038, 0x0303);
  Lcd_Write_Com_Data(0x0039, 0x0707);
  Lcd_Write_Com_Data(0x003C, 0x0504);
  Lcd_Write_Com_Data(0x003D, 0x0808);
  //------------------ Set GRAM area ---------------//
  Lcd_Write_Com_Data(0x0050, 0x0000); // Horizontal GRAM Start Address
  Lcd_Write_Com_Data(0x0051, 0x00EF); // Horizontal GRAM End Address
  Lcd_Write_Com_Data(0x0052, 0x0000); // Vertical GRAM Start Address
  Lcd_Write_Com_Data(0x0053, 0x013F); // Vertical GRAM Start Address
  Lcd_Write_Com_Data(0x0060, 0xA700); // Gate Scan Line
  Lcd_Write_Com_Data(0x0061, 0x0001); // NDL,VLE, REV
 
  Lcd_Write_Com_Data(0x006A, 0x0000); // set scrolling line
  //-------------- Partial Display Control ---------//
  Lcd_Write_Com_Data(0x0080, 0x0000);
  Lcd_Write_Com_Data(0x0081, 0x0000);
  Lcd_Write_Com_Data(0x0082, 0x0000);
  Lcd_Write_Com_Data(0x0083, 0x0000);
  Lcd_Write_Com_Data(0x0084, 0x0000);
  Lcd_Write_Com_Data(0x0085, 0x0000);
  //-------------- Panel Control -------------------//
  Lcd_Write_Com_Data(0x0090, 0x0010);
  Lcd_Write_Com_Data(0x0092, 0x0000);
  Lcd_Write_Com_Data(0x0007, 0x0133); // 262K color and display ON
        digitalWrite(LCD_CS,HIGH);
 
}
 
void Pant(char VH,char VL)
{
  int i,j;
  digitalWrite(LCD_CS,LOW); 
  Address_set(0,0,240,320);
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
  for(int p=0;p<20;p++)
  {
    pinMode(p,OUTPUT);
  }
  Lcd_Init();          
}
 
void loop()
{
   Pant(0x0, 0x0);
   delay(500);
   Pant(0xff, 0x00);
   delay(500);
   Pant(0x00, 0xff);
   delay(500);
   Pant(0xff, 0xff);
   delay(500);
}
