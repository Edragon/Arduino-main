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

unsigned char Dis_table[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};	//LED状态显示的变量
unsigned char Dis_buf[]   = {0xF1,0xF2,0xF4,0xF8};
unsigned char disbuff[]  =  {0, 0, 0, 0};
unsigned int  add[65]={0};//宝义一个数组存放电压数值

int rT=0;
float temp;   //创建一个浮点型变量temp作为存储空间准备存放数据



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
    if(i==0)
    {
    shiftOut(dataPin,clockPin,MSBFIRST,Dis_table[disbuff[i]]&0x7F); //发送显示码
    }
    else
    shiftOut(dataPin,clockPin,MSBFIRST,Dis_table[disbuff[i]]);
    shiftOut(dataPin,clockPin,MSBFIRST,Dis_buf[i] );  //发送通值													//串行数据输入引脚为dataPin，时钟引脚为clockPin，执行MSB有限发送，发送数据table[i]
    digitalWrite(latchPin,HIGH); //将ST_CP这个针脚恢复到高电平
    delay(2);				//延时2ms 
  }
  
}

void loop()
{
  float vol = analogRead(A0)*(4.97 / 1023.0); //我们将 V1的值换算成实际电压值存入浮点型变量 vol
  rT= (int)(vol*1000);  
  disbuff[0]= rT/1000;
  disbuff[1]= rT%1000/100;
  disbuff[2]= rT%100/10;
  disbuff[3]= rT%10;
 
  for(char time=0;time<30;time++)  //显示30次,当作延时,以免读取过快
 { 
  display();
 }
}

