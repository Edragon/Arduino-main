#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS A4
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);


int rT=0;

int latchPin = 4;
int clockPin =7;
int dataPin = 8; //这里定义了那三个脚

unsigned char Dis_table[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};	//LED状态显示的变量
unsigned char Dis_buf[]   = {0xF1,0xF2,0xF4,0xF8};
unsigned char disbuff[]  =  {0, 0, 0, 0};



void setup ()
{
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT); //让三个脚都是输出状态
  sensors.begin();
}

void display()
{
  for(char i=0; i<=3; i++)//扫描四个数码管
  {
    digitalWrite(latchPin,LOW); //将ST_CP口上面加低电平让芯片准备好接收数据
    if(i==1)
    {
    shiftOut(dataPin,clockPin,MSBFIRST,Dis_table[disbuff[i]]&0x7F); //发送显示码 0-3
    }
     else
     
      shiftOut(dataPin,clockPin,MSBFIRST,Dis_table[disbuff[i]]);
    shiftOut(dataPin,clockPin,MSBFIRST,Dis_buf[i] );  //发送通值													//串行数据输入引脚为dataPin，时钟引脚为clockPin，执行MSB有限发送，发送数据table[i]
    digitalWrite(latchPin,HIGH); //将ST_CP这个针脚恢复到高电平
    delay(2);				//延时2ms 
  }
  
}


void loop(void)
{ 
       sensors.requestTemperatures(); // Send the command to get temperatures
  float t=sensors.getTempCByIndex(0);
 
        rT= (int)(t*100);   //放大100位转化为INT型
        disbuff[0]= rT/1000;
        disbuff[1]= rT%1000/100;
        disbuff[2]= rT%1000%100/10;
        disbuff[3]= rT%1000%100%10;
        for(char time=0;time<20;time++)
        {
        display();         //调用显示程序
        }
}

