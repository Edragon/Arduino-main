#define LCD_RS   19       // A5  
#define LCD_WR   18       // A4
#define LCD_CS   17       // A3
#define LCD_REST 16       // A2

#define DCLK     15       // A1
#define DIN      14       // A0
#define CS       8
#define DOUT     9
//#define IRQ      8

unsigned int TP_X, TP_Y;
unsigned int TouchCount = 0;
int  pacy = 0;



void spistar()                                     //SPI Start
{
  digitalWrite(CS, LOW);
  digitalWrite(DCLK, HIGH);
  digitalWrite(DIN, HIGH);
  digitalWrite(DCLK, HIGH);

}
//**********************************************************
void WriteCharTo7843(unsigned char num)          //SPI Write Data
{
  unsigned char count = 0;
  unsigned char temp;
  unsigned nop;
  temp = num;
  digitalWrite(DCLK, LOW);
  for (count = 0; count < 8; count++)
  {
    if (temp & 0x80)
      digitalWrite(DIN, HIGH);
    else
      digitalWrite(DIN, LOW);

    temp = temp << 1;

    digitalWrite(DCLK, LOW);
    nop++;
    nop++;
    digitalWrite(DCLK, HIGH);
    nop++;
    nop++;
  }
}

//**********************************************************
unsigned int ReadFromCharFrom7843()             //SPI Read Data
{
  unsigned nop;
  unsigned char count = 0;
  unsigned int Num = 0;
  for (count = 0; count < 12; count++)
  {
    Num <<= 1;
    digitalWrite(DCLK, HIGH); //DCLK=1; _nop_();_nop_();_nop_();
    nop++;
    digitalWrite(DCLK, LOW); //DCLK=0; _nop_();_nop_();_nop_();
    nop++;
    if (digitalRead(DOUT)) Num++;
  }
  return (Num);
}



void LCD_Writ_Bus(char VH, char VL)
{
  PORTD = VH;
  digitalWrite(LCD_WR, LOW);
  digitalWrite(LCD_WR, HIGH);
  PORTD = VL;
  digitalWrite(LCD_WR, LOW);
  digitalWrite(LCD_WR, HIGH);
}


void LCD_Write_COM(char VH, char VL)
{
  digitalWrite(LCD_RS, LOW);
  LCD_Writ_Bus(VH, VL);
}


void LCD_Write_DATA(char VH, char VL)
{
  digitalWrite(LCD_RS, HIGH);
  LCD_Writ_Bus(VH, VL);
}

void Lcd_Write_Com_Data(int com, int val) {     //鍙戦€佹暟鎹懡浠?{
  LCD_Write_COM(com >> 8, com);
  LCD_Write_DATA(val >> 8, val);
}

void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
  LCD_Write_COM(0x00, 0x46); LCD_Write_DATA(x2, x1);
  LCD_Write_COM(0x00, 0x47); LCD_Write_DATA(y2 >> 8, y2);
  LCD_Write_COM(0x00, 0x48); LCD_Write_DATA(y1 >> 8, y1);
  LCD_Write_COM(0x00, 0x20); LCD_Write_DATA(x1 >> 8, x1);
  LCD_Write_COM(0x00, 0x21); LCD_Write_DATA(y1 >> 8, y1);
  LCD_Write_COM(0x00, 0x22);
}

void LCD_Init(void)
{

  digitalWrite(LCD_REST, HIGH);
  delay(5);
  digitalWrite(LCD_REST, LOW);
  delay(5);
  digitalWrite(LCD_REST, HIGH);
  delay(5);

  digitalWrite(LCD_CS, LOW);
  Lcd_Write_Com_Data(0x11, 0x2004);
  Lcd_Write_Com_Data(0x13, 0xCC00);
  Lcd_Write_Com_Data(0x15, 0x2600);
  Lcd_Write_Com_Data(0x14, 0x252A);
  Lcd_Write_Com_Data(0x12, 0x0033);
  Lcd_Write_Com_Data(0x13, 0xCC04);
  delay(1);
  Lcd_Write_Com_Data(0x13, 0xCC06);
  delay(1);
  Lcd_Write_Com_Data(0x13, 0xCC4F);
  delay(1);
  Lcd_Write_Com_Data(0x13, 0x674F);
  Lcd_Write_Com_Data(0x11, 0x2003);
  delay(1);
  Lcd_Write_Com_Data(0x30, 0x2609);
  Lcd_Write_Com_Data(0x31, 0x242C);
  Lcd_Write_Com_Data(0x32, 0x1F23);
  Lcd_Write_Com_Data(0x33, 0x2425);
  Lcd_Write_Com_Data(0x34, 0x2226);
  Lcd_Write_Com_Data(0x35, 0x2523);
  Lcd_Write_Com_Data(0x36, 0x1C1A);
  Lcd_Write_Com_Data(0x37, 0x131D);
  Lcd_Write_Com_Data(0x38, 0x0B11);
  Lcd_Write_Com_Data(0x39, 0x1210);
  Lcd_Write_Com_Data(0x3A, 0x1315);
  Lcd_Write_Com_Data(0x3B, 0x3619);
  Lcd_Write_Com_Data(0x3C, 0x0D00);
  Lcd_Write_Com_Data(0x3D, 0x000D);
  Lcd_Write_Com_Data(0x16, 0x0007);
  Lcd_Write_Com_Data(0x02, 0x0013);
  Lcd_Write_Com_Data(0x03, 0x0003);
  Lcd_Write_Com_Data(0x01, 0x0127);
  delay(1);
  Lcd_Write_Com_Data(0x08, 0x0303);
  Lcd_Write_Com_Data(0x0A, 0x000B);
  Lcd_Write_Com_Data(0x0B, 0x0003);
  Lcd_Write_Com_Data(0x0C, 0x0000);
  Lcd_Write_Com_Data(0x41, 0x0000);
  Lcd_Write_Com_Data(0x50, 0x0000);
  Lcd_Write_Com_Data(0x60, 0x0005);
  Lcd_Write_Com_Data(0x70, 0x000B);
  Lcd_Write_Com_Data(0x71, 0x0000);
  Lcd_Write_Com_Data(0x78, 0x0000);
  Lcd_Write_Com_Data(0x7A, 0x0000);
  Lcd_Write_Com_Data(0x79, 0x0007);
  Lcd_Write_Com_Data(0x07, 0x0051);
  delay(1);
  Lcd_Write_Com_Data(0x07, 0x0053);
  Lcd_Write_Com_Data(0x79, 0x0000);

  LCD_Write_COM(0x00, 0x22);
  digitalWrite(LCD_CS, HIGH);

}

void Pant(char VH, char VL)
{
  int i, j;
  digitalWrite(LCD_CS, LOW);
  Address_set(0, 0, 240, 320);
  for (i = 0; i <= 320; i++)
  {
    for (j = 0; j <= 240; j++)
    {
      LCD_Write_DATA(VH, VL);
    }
  }
  digitalWrite(LCD_CS, HIGH);
}


void AD7843(void)
{
  digitalWrite(CS, LOW);
  WriteCharTo7843(0x90);
  digitalWrite(DCLK, HIGH);
  digitalWrite(DCLK, LOW);
  TP_Y = ReadFromCharFrom7843();
  WriteCharTo7843(0xD0);
  digitalWrite(DCLK, HIGH);
  digitalWrite(DCLK, LOW);
  TP_X = ReadFromCharFrom7843();
  digitalWrite(CS, HIGH);
}




void setup()
{

  unsigned char p;
  int i, j, k;
  for (p = 0; p < 20; p++)
  {
    pinMode(p, OUTPUT);
  }
  pinMode(DOUT, INPUT);
  //  pinMode(IRQ,INPUT);

  LCD_Init();
  Pant(0x00, 0x00);
}

void loop() {
  TouchCount++;
  unsigned char flag;
  unsigned char ss[6];
  unsigned int lx, ly;
  spistar();
  if (TouchCount == 200)
  {
    pacy = random(0, 7);
    TouchCount = 0;
  }

  //  while(digitalRead(IRQ)==0)
  //  {
  digitalWrite(LCD_CS, LOW);
  AD7843();
  lx = 240 - ((TP_X - 220) / 16);
  ly = 320 - ((TP_Y - 400) / 11);
  Address_set(lx, ly, lx + 2, ly + 2);
  switch (pacy)
  {
    case 0: for (int i = 0; i < 5; i++)  LCD_Write_DATA(0xF8, 0x00);  break; //Red
    case 1: for (int i = 0; i < 5; i++)  LCD_Write_DATA(0xFF, 0xE0);  break; //Yellow
    case 2: for (int i = 0; i < 5; i++)  LCD_Write_DATA(0xFF, 0xFF);  break; //White
    case 3: for (int i = 0; i < 5; i++)  LCD_Write_DATA(0x05, 0x1F);  break; //Blue
    case 4: for (int i = 0; i < 5; i++)  LCD_Write_DATA(0x00, 0x1F);  break; //Blue-2
    case 5: for (int i = 0; i < 5; i++)  LCD_Write_DATA(0xF8, 0x1F);  break; //Magenta
    case 6: for (int i = 0; i < 5; i++)  LCD_Write_DATA(0x07, 0xE0);  break; //Green
    case 7: for (int i = 0; i < 5; i++)  LCD_Write_DATA(0x7F, 0xFF);  break; //Cyan
defoult: for (int i = 0; i < 5; i++)  LCD_Write_DATA(0x00, 0x00);  break; //Black
  }
  digitalWrite(LCD_CS, HIGH);
  //    }
}

