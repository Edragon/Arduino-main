int zdpin = 0;              //中断0，对应引脚UNO的D2
volatile int zt = LOW;

int dim_ctrl = 4;

int dim_time = 1000;
//int m=3000;                        //设置延时初始值，单位微秒
//int mx=5200;                //设置延时最大值，单位微秒
//int mi=900;                        //设置延时最小值，单位微秒

//int x=0;                        //按键
//int y=0;                        //按键

void setup ()
{
  pinMode(3, OUTPUT);               //光耦控制端
  //pinMode(5,INPUT);                        //按键
  //pinMode(6,INPUT);                        //按键
  pinMode(13, OUTPUT);
  
  attachInterrupt(zdpin, light, RISING);              //过零检测的中断
  //Serial.begin(9600);
  pinMode(dim_ctrl, INPUT);
  digitalWrite(dim_ctrl, HIGH);
}

void loop()
{
  //  x=digitalRead(5);                //读取按键状态，D6
  //  delayMicroseconds(100);                        //防抖
  //  y=digitalRead(6);                //读取按键状态，D5
  //  delayMicroseconds(100);                        //防抖
  //
  //  if (x==1)                //最大延时累加，以及封顶
  //  {
  //   m=m+20;
  //   if (m>=mx)
  //   {
  //   m=mx;
  //   }
  //  }
  //
  //  if (y==1)
  //  {
  //   m=m-20;                //最小延时累减，以及封底
  //   if (m<=mi)
  //   {
  //   m=mi;
  //   }
  //  }
  //delayMicroseconds(3000);                        //过零检测延时
  if (digitalRead(dim_ctrl) == HIGH) {
    dim_time = 1000; 
    digitalWrite(13, HIGH);
  } else {
    dim_time = 4000; 
    digitalWrite(13, LOW);
  }
  
  delayMicroseconds(100);
  digitalWrite(11, 0);               //默认低电位

  //  Serial.println(3000);
}

void light()
{
  delayMicroseconds(dim_time);                        //过零检测延时
  digitalWrite(3, 1);                       //控制光耦导通
}
