//*******************************************************************************
//----------------本例程仅供学习使用，未经作者允许，不得用于其他用途。-----------
//------------------------版权所有，仿冒必究！-----------------------------------
//----------------1.开发环境:Arduino IDE-----------------------------------------
//----------------2.测试使用开发板型号：Arduino Leonardo or Arduino UNO R3-------
//----------------3.单片机使用晶振：16M------------------------------------------
//----------------4.淘宝网址：http://xmdzpj.taobao.com---------------------------
//----------------5.作者：真红炎神---------------------------------------------
//*******************************************************************************/
#include <Arduino.h>

#define A1 5			//引脚命名
#define B1 6
#define C1 9
#define D1 A5


void setup()
{
	pinMode(A1,OUTPUT);		//设置引脚为输出引脚
	pinMode(B1,OUTPUT);
	pinMode(C1,OUTPUT);
	pinMode(D1,OUTPUT);
}

void loop()
{
	Phase_A();			//设置A相位
	delay(10);			//改变延时可改变旋转速度

	Phase_B();			//设置B相位
	delay(10);

	Phase_C();			//设置C相位
	delay(10);

	Phase_D();			//设置D相位
	delay(10);

}

void Phase_A()
{
	digitalWrite(A1,HIGH);		//A1引脚高电平	
	digitalWrite(B1,LOW);
	digitalWrite(C1,LOW);
	digitalWrite(D1,LOW);
}

void Phase_B()
{
	digitalWrite(A1,LOW);	
	digitalWrite(B1,HIGH);		//B1引脚高电平	
	digitalWrite(C1,LOW);
	digitalWrite(D1,LOW);
}

void Phase_C()
{
	digitalWrite(A1,LOW);	
	digitalWrite(B1,LOW);
	digitalWrite(C1,HIGH);		//C1引脚高电平	
	digitalWrite(D1,LOW);
}

void Phase_D()
{
	digitalWrite(A1,LOW);	
	digitalWrite(B1,LOW);
	digitalWrite(C1,LOW);
	digitalWrite(D1,HIGH);		//D1引脚高电平	
}
