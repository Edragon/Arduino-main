int ledpin=13;//定义数字30 接口
int inpin=A3;  //定义数字5 接口
int val;//定义变量val
void setup()
{
pinMode(ledpin,OUTPUT);//定义小灯接口为输出接口
pinMode(inpin,INPUT);//定义按键接口为输入接口
}
void loop()
{
val=digitalRead(inpin);//读取数字5 口电平值赋给val
if(val==LOW)           //检测按键是否按下，按键按下时小灯亮起
{ digitalWrite(ledpin,LOW);}
else
{ digitalWrite(ledpin,HIGH);}
}

