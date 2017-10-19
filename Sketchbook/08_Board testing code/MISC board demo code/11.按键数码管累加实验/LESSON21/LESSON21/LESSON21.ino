//*******************************************************************************
//----------------本例程仅供学习使用，未经作者允许，不得用于其他用途。-----------
//------------------------版权所有，仿冒必究！-----------------------------------
//----------------1.开发环境:Arduino IDE-----------------------------------------
//----------------2.测试使用开发板型号：Arduino Leonardo or Arduino UNO R3-------
//----------------3.单片机使用晶振：16M------------------------------------------
//----------------4.淘宝网址：http://xmdzpj.taobao.com---------------------------
//----------------5.作者：真红炎神---------------------------------------------
//*******************************************************************************/
int latchPin = 4;
int clockPin =7;
int dataPin = 8; //这里定义了那三个脚
int KEY_ADD =A1;  //定义按键加
int KEY_DEC=A2;  //定义按键减
unsigned char Dis_table[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};	//LED状态显示的变量
unsigned char Dis_buf[]   = {0xF1,0xF2,0xF4,0xF8};
unsigned char disbuff[]  =  {0, 0, 0, 0};
int SUM = 0;
int Flag_up = 1;
int Flag_up1 = 1;

void setup ()
{
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT); //让三个脚都是输出状态
}

void display()
{
  for(char i=0; i<=3; i++)//扫描四个数码管
  {
    digitalWrite(latchPin,LOW); //将ST_CP口上面加低电平让芯片准备好接收数据
    shiftOut(dataPin,clockPin,MSBFIRST,Dis_table[disbuff[i]]); //发送显示码 0-3
    shiftOut(dataPin,clockPin,MSBFIRST,Dis_buf[i] );  //发送通值													//串行数据输入引脚为dataPin，时钟引脚为clockPin，执行MSB有限发送，发送数据table[i]
    digitalWrite(latchPin,HIGH); //将ST_CP这个针脚恢复到高电平
    delay(2);				//延时2ms 
  }
  
}
unsigned char ScanKey()			//按键扫描程序，返回键值
{
  if(Flag_up  && digitalRead(KEY_ADD) == LOW)		//当Flag_up = 1并且按键按下时
  {
    Flag_up = 0;										//清空Flag_up
    display();
    display();											//延时去抖动
    if(digitalRead(KEY_ADD) == LOW)			//按键按下
    {
      return 1;											//返回键值1
    }
  }
  if(digitalRead(KEY_ADD) == HIGH)			//按键松开时候
  {
    Flag_up = 1;										//置位Flag_up
  }
  return 0;													//返回键值0
}

unsigned char ScanKey1()			//按键扫描程序，返回键值
{
  if(Flag_up1  && digitalRead(KEY_DEC) == LOW)		//当Flag_up = 1并且按键按下时
  {
    Flag_up1 = 0;										//清空Flag_up
    display();
    display();											//延时去抖动
    if(digitalRead(KEY_DEC) == LOW)			//按键按下
    {
      return 1;											//返回键值1
    }
  }
  if(digitalRead(KEY_DEC) == HIGH)			//按键松开时候
  {
    Flag_up1 = 1;										//置位Flag_up
  }
  return 0;													//返回键值0
}

void loop()
{
  
   display();
   if( ScanKey() == 1)				//当有按键按下时
  {
    SUM++;								//SUM加一
    if(SUM>9999)					//最大值9999，不再累加
    {
      SUM = 9999;
    }
    disbuff[0]=SUM/1000;		//显示SUM千位值
    disbuff[1]=SUM%1000/100;	//显示SUM百位值
    disbuff[2]=SUM%100/10;		//显示SUM十位值
    disbuff[3]=SUM%10;				//显示SUM个位值
   }
   
   if( ScanKey1() == 1)				//当有按键按下时
  {
    SUM--;								//SUM加一
    if(SUM<=0)					//最大值9999，不再累加
    {
      SUM = 0;
    }
    disbuff[0]=SUM/1000;		//显示SUM千位值
    disbuff[1]=SUM%1000/100;	//显示SUM百位值
    disbuff[2]=SUM%100/10;		//显示SUM十位值
    disbuff[3]=SUM%10;				//显示SUM个位值
   }
  
  }
  
  

