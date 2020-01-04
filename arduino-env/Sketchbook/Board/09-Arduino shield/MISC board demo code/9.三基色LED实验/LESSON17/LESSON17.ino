//*******************************************************************************
//----------------本例程仅供学习使用，未经作者允许，不得用于其他用途。-----------
//------------------------版权所有，仿冒必究！-----------------------------------
//----------------1.开发环境:Arduino IDE-----------------------------------------
//----------------2.测试使用开发板型号：Arduino Leonardo or Arduino UNO R3-------
//----------------3.单片机使用晶振：16M------------------------------------------
//----------------4.淘宝网址：http://xmdzpj.taobao.com---------------------------
//----------------5.作者：真红炎神---------------------------------------------
//*******************************************************************************/
#define LED_R 2
#define LED_G 3
#define LED_B 4

enum{Color_R,Color_G,Color_B,Color_RG,Color_RB,Color_GB,Color_RGB};	//枚举所有的颜色
//红，绿，蓝，黄，紫，青，白

void setup()
{
	pinMode(LED_R,OUTPUT);
	pinMode(LED_G,OUTPUT);
	pinMode(LED_B,OUTPUT);
}

void loop()
{
	unsigned int i;
	for(i = 0 ; i < 7 ; i++)			//七种颜色选择
	{
  	  Change_Color(i);			//选择第i种颜色显示
      delay(1000);					//延时1000ms
	}
}

void Change_Color(unsigned char data_color)			//选择颜色函数
{
	switch(data_color)
	{
		case Color_R:							//红色
			digitalWrite(LED_R,LOW);
			digitalWrite(LED_G,HIGH);
			digitalWrite(LED_B,HIGH);
			break;
		case Color_G:							//绿色
			digitalWrite(LED_R,HIGH);
			digitalWrite(LED_G,LOW);
			digitalWrite(LED_B,HIGH);
			break;
		case Color_B:							//蓝色
			digitalWrite(LED_R,HIGH);
			digitalWrite(LED_G,HIGH);
			digitalWrite(LED_B,LOW);
			break;
		case Color_RG:							//黄色
			digitalWrite(LED_R,LOW);
			digitalWrite(LED_G,LOW);
			digitalWrite(LED_B,HIGH);
			break;
		case Color_RB:							//紫色
			digitalWrite(LED_R,LOW);
			digitalWrite(LED_G,HIGH);
			digitalWrite(LED_B,LOW);
			break;
		case Color_GB:							//青色
			digitalWrite(LED_R,HIGH);
			digitalWrite(LED_G,LOW);
			digitalWrite(LED_B,LOW);
			break;
		case Color_RGB:							//白色
			digitalWrite(LED_R,LOW);
			digitalWrite(LED_G,LOW);
			digitalWrite(LED_B,LOW);
			break;
		default:
			break;

	}

}
