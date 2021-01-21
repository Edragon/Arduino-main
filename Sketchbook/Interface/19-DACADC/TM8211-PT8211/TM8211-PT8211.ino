//定义引脚，可以自行修改引脚对应
int BCK = 10, WS = 9, DIN = 8;
unsigned int cnt = 0;

//TM8211驱动函数，
void TM8211(unsigned int wr_dat8211)
{
    for(int i = 16; i > 0; i--)
    {
        digitalWrite(BCK, LOW);
        digitalWrite(DIN, (wr_dat8211 >> i) & 0x01);
        digitalWrite(BCK, HIGH);
    }
}

void setup()
{
    //所有引脚都是输出方式
    pinMode(BCK, OUTPUT);
    pinMode(WS, OUTPUT);
    pinMode(DIN, OUTPUT);
}

void loop()
{
    //cnt是DA值
    if(cnt > 0xFFF0)
    {
        cnt = 0;
    }
    //WS是通道选择，这段代码是两个通道LCH/RCH同时输出cnt，也可以分开LCH/RCH控制的，具体看pdf
    digitalWrite(WS, LOW);
    TM8211(cnt);
    digitalWrite(WS, HIGH);
    digitalWrite(WS, LOW);
    delay(1);
    cnt += 0x0F;    //每次输出增加16个等级
}
