//*******************************************************************************
//----------------本例程仅供学习使用，未经作者允许，不得用于其他用途。-----------
//------------------------版权所有，仿冒必究！-----------------------------------
//----------------1.开发环境:Arduino IDE-----------------------------------------
//----------------2.测试使用开发板型号：Arduino Leonardo or Arduino UNO R3-------
//----------------3.单片机使用晶振：16M------------------------------------------
//----------------4.淘宝网址：http://xmdzpj.taobao.com---------------------------
//----------------5.作者：真红炎神---------------------------------------------
//*******************************************************************************/
char ledPin =  10; //定义数字10 接口
char ledPin1 = 11; //定义数字11 接口
char ledPin2 = 12; //定义数字12 接口
char ledPin3 = 13; //定义数字13 接口
void setup()
{
pinMode(ledPin, OUTPUT);//定义小灯接口为输出接口
pinMode(ledPin1, OUTPUT);//定义小灯接口为输出接口
pinMode(ledPin2, OUTPUT);//定义小灯接口为输出接口
pinMode(ledPin3, OUTPUT);//定义小灯接口为输出接口
}
void loop()
{
digitalWrite(ledPin, HIGH); //点亮小灯
digitalWrite(ledPin1, HIGH); //点亮小灯
digitalWrite(ledPin2, HIGH); //点亮小灯
digitalWrite(ledPin3, HIGH); //点亮小灯
delay(1000); //延时1 秒
digitalWrite(ledPin, LOW); //熄灭小灯
digitalWrite(ledPin1, LOW); //熄灭小灯
digitalWrite(ledPin2, LOW); //熄灭小灯
digitalWrite(ledPin3, LOW); //熄灭小灯
delay(1000); // 延时1 秒
}

