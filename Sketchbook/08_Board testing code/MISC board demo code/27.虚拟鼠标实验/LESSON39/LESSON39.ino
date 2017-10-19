//*******************************************************************************
//----------------本例程仅供学习使用，未经作者允许，不得用于其他用途。-----------
//------------------------版权所有，仿冒必究！-----------------------------------
//----------------1.开发环境:Arduino IDE-----------------------------------------
//----------------2.测试使用开发板型号：Arduino Leonardo or Arduino UNO R3-------
//----------------3.单片机使用晶振：16M------------------------------------------
//----------------4.淘宝网址：http://xmdzpj.taobao.com---------------------------
//----------------5.作者：真红炎神---------------------------------------------
//*******************************************************************************/
#define KEY1 A1			//左移
#define KEY2 A2			//右移
#define KEY3 A3      //左键 

int Flag_up = 1;

void setup() 
{
  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);
  pinMode(KEY3, INPUT_PULLUP);
  Mouse.begin();
}
 
void loop() 
{
  if( ScanKey(1) == 1)			//左移
  {
    Mouse.move(-10, 0);		//左移40
  }
  else if(ScanKey(1) == 2)	//右移
  {
    Mouse.move(10, 0);	
  }
  else if(ScanKey(1) == 3)	//左键按下
  {
    Mouse.click(MOUSE_LEFT);	
  }
}

unsigned char ScanKey(int mode)			//按键扫描程序，返回键值
{
  if(mode)							//mode = 1时候，该代码支持长按。mode = 0时候，不支持长按
  {
    Flag_up = 1;
  }
  
  if(Flag_up  && (digitalRead(KEY1) == LOW || digitalRead(KEY2) == LOW || digitalRead(KEY3) == LOW) )		//当Flag_up = 1并且按键按下时
  {
    Flag_up = 0;										//清空Flag_up
    delay(30);											//延时去抖动
    if(digitalRead(KEY1) == LOW)			//按键按下
    {
      return 1;											//返回键值1
    }
    if(digitalRead(KEY2) == LOW)			//按键按下
    {
      return 2;											//返回键值2
    }
    if(digitalRead(KEY3) == LOW)			//按键按下
    {
      return 3;											//返回键值3
    }
  }
  if(digitalRead(KEY1) == HIGH && digitalRead(KEY2) == HIGH && digitalRead(KEY3) == HIGH)			//按键松开时候
  {
    Flag_up = 1;										//置位Flag_up
  }
  return 0;													//返回键值0
}








