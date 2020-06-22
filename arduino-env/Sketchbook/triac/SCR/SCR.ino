//D2接模块OUT，D3接模块的IN
int detectado = 0;
int valor=0;
int last_CH1_state = 0;

void setup() {
  PCICR |= (1 << PCIE0);    //启用PCMSK0扫描                                               
  PCMSK0 |= (1 << PCINT0);  //设置引脚D8触发中断状态改变,光耦输入.
  pinMode(3,OUTPUT);        //将D3定义为DIAC脉冲的输出

}

void loop() {
   valor = analogRead(A0);
   valor = map(analogRead(A0),0,1024,10000,2188);
    if (detectado)
    {
      delayMicroseconds(valor); //此延迟控制电源
      digitalWrite(3,HIGH);
      delayMicroseconds(100);
      digitalWrite(3,LOW);
      detectado=0;
    } 
}

//======================中断程序=====================================
ISR(PCINT0_vect){
  /////////////////////////////////////               //光耦输入
  if(PINB & B00000001){                               //我们用引脚状态寄存器进行AND运算，验证引脚8是否为高电平？
    if(last_CH1_state == 0){                          //如果最后一个状态为0，那么我们将进行状态更改...
      detectado=1;                                    //我们发现了状态变化!
    }
  }
  else if(last_CH1_state == 1){                       //如果引脚8为低电平且最后一个状态为高电平，则表示状态发生变化     
    detectado=1;                                      //我们发现了状态变化！
    last_CH1_state = 0;                               //将当前状态存储到下一个循环的最后一个状态
    }
}
