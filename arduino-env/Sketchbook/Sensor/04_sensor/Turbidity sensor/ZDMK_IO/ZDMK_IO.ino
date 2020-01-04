int ledPin = 13;                // 控制D13口的LED
int in = 2;                 // 读取传感器PIN端口
void setup(){
  pinMode(ledPin, OUTPUT);      // 设置LED为输出模式
  pinMode(in, INPUT);       //设置传感器为读取模式
}
void loop(){
   if(digitalRead(in)==LOW){       //读传感器低电平就点亮LED
        digitalWrite(ledPin, HIGH);   // 点亮LED
     }else{
        digitalWrite(ledPin, LOW);    // 如果不是低电平就关闭LED
     }
}
