//*******************************************************************************
//----------------本例程仅供学习使用，未经作者允许，不得用于其他用途。-----------
//------------------------版权所有，仿冒必究！-----------------------------------
//----------------1.开发环境:Arduino IDE-----------------------------------------
//----------------2.测试使用开发板型号：Arduino Leonardo or Arduino UNO R3-------
//----------------3.单片机使用晶振：16M------------------------------------------
//----------------4.淘宝网址：http://xmdzpj.taobao.com---------------------------
//----------------5.作者：真红炎神---------------------------------------------
//*******************************************************************************/
int servopin=5;//定义数字接口5 连接伺服舵机信号线
int myangle;//定义角度变量
int pulsewidth;//定义脉宽变量
int val;
void servopulse(int servopin,int myangle)//定义一个脉冲函数
{
 pulsewidth=(myangle*11)+500;//将角度转化为500-2480 的脉宽值
 digitalWrite(servopin,HIGH);//将舵机接口电平至高
 delayMicroseconds(pulsewidth);//延时脉宽值的微秒数
 digitalWrite(servopin,LOW);//将舵机接口电平至低
 delay(20-pulsewidth/1000);
}
void setup()
{
 pinMode(servopin,OUTPUT);//设定舵机接口为输出接口
 Serial.begin(9600);//连接到串行端口，波特率为9600
 Serial.println("servo=o_seral_simple ready" ) ;
}
void loop()//将0 到9 的数转化为0 到180 角度，并让LED 闪烁相应数的次数
{
   val=Serial.read();//读取串行端口的值
if(val>'0'&&val<='9')
{
  val=val-'0';//将特征量转化为数值变量
  val=val*(180/9);//将数字转化为角度
  Serial.print("moving servo to ");
  Serial.print(val,DEC);
  Serial.println();
for(int i=0;i<=50;i++) //给予舵机足够的时间让它转到指定角度
{
  servopulse(servopin,val);//引用脉冲函数
}
}
}

