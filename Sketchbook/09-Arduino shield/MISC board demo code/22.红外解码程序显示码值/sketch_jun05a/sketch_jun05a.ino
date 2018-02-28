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
void setup()
{  
  Serial.begin(9600);   // 开启串口，波特率为9600  
  irrecv.enableIRIn();   // 启动红外解码
} 
void loop() 
{  
  if (irrecv.decode(&results)) 
{   // 解码成功，把数据放入results变量中    // 把数据输入到串口   
  Serial.print("irCode: ");           
  Serial.print(results.value, HEX); // 显示红外编码  
  Serial.print(",  bits: ");              
  Serial.println(results.bits); // 显示红外编码位数   0
  irrecv.resume();    // 继续等待接收下一组信号 
  }    
// delay(600); //延时600毫秒，做一个简单的消抖
 }
