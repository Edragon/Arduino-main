//*******************************************************************************
//----------------本例程仅供学习使用，未经作者允许，不得用于其他用途。-----------
//------------------------版权所有，仿冒必究！-----------------------------------
//----------------1.开发环境:Arduino IDE-----------------------------------------
//----------------2.测试使用开发板型号：Arduino Leonardo or Arduino UNO R3-------
//----------------3.单片机使用晶振：16M------------------------------------------
//----------------4.淘宝网址：http://xmdzpj.taobao.com---------------------------
//----------------5.作者：真红炎神---------------------------------------------
//*******************************************************************************/

#include <IRremote.h>         // 使用IRRemote函数库 
const int irReceiverPin = 2;  // 红外接收器的 OUTPUT 引脚接在 PIN2 接口 定义irReceiverPin变量为PIN2接口
IRrecv irrecv(irReceiverPin); // 设置irReceiverPin定义的端口为红外信号接收端口
decode_results results;       // 定义results变量为红外结果存放位置 
char LEDpin=13;
boolean  flag=1;
void setup()
{  
  Serial.begin(9600);   // 开启串口，波特率为9600  
  irrecv.enableIRIn();   // 启动红外解码
  pinMode(LEDpin,OUTPUT);
  digitalWrite(LEDpin,HIGH);
} 
void loop() 
{  
  if (irrecv.decode(&results)) 
{   // 解码成功，把数据放入results变量中    // 把数据输入到串口   
  switch(results.value)
{  
  case 0xffa05f: Serial.println("OK");  break;//OK
  case 0xff708f: Serial.println("go");  break;//向上
  case 0xff58a7: Serial.println("back");break;//向下
  case 0xff50af: Serial.println("left");break;//向左
  case 0xff7887: Serial.println("right");break;//向右
  case 0xffb04f: digitalWrite(LEDpin,flag=!flag);Serial.println(flag, HEX);break;//1
  case 0xff9867: Serial.println(2, HEX);break;//2
  case 0xffd827: Serial.println(3, HEX);break;//3
  case 0xff807f: Serial.println(4, HEX);break;//4
  case 0xff8877: Serial.println(5, HEX);break;//5    
  case 0xffa857: Serial.println(6, HEX);break;//6
  case 0xffe817: Serial.println(7, HEX);break;//7 
  case 0xff609f: Serial.println(8, HEX);break;//8 
  case 0xff48b7: Serial.println(9, HEX); break;//9 
}
 irrecv.resume();    // 继续等待接收下一组信号 
  }    

 }
