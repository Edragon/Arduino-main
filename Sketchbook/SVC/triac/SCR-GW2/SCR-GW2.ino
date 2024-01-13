int zdpin = 0;              //中断0，对应引脚UNO的D2
volatile int zt = LOW;

int dim_ctrl = 4;

int dim_time = 1000;

void setup ()
{
  pinMode(3, OUTPUT);               //光耦控制端
  
  pinMode(13, OUTPUT); // indicator
  
  pinMode(dim_ctrl, INPUT);
  digitalWrite(dim_ctrl, HIGH);
   
  attachInterrupt(zdpin, light, RISING);              //过零检测的中断

}

void loop()
{
  if (digitalRead(dim_ctrl) == HIGH) { // button D4
    dim_time = 1000; 
    digitalWrite(13, HIGH);
  } else {
    dim_time = 4000; 
    digitalWrite(13, LOW);
  }
  
  delayMicroseconds(100);
  digitalWrite(11, 0);               //默认低电位

}

void light()
{
  delayMicroseconds(dim_time);                        //过零检测延时
  digitalWrite(3, 1);                       //控制光耦导通
}
