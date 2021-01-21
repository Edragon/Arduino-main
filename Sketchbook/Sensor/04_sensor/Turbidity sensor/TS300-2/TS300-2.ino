int ledPin = 13; // 控制 D13 口的 LED
int in = 2; // 读取传感器 PIN 端口
void setup() {
  pinMode(ledPin, OUTPUT); // 设置 LED 为输出模式
  pinMode(in, INPUT); //设置传感器为读取模式
}
void loop() {
  if (digitalRead(in) == LOW) { //读传感器低电平就点亮 LED
    digitalWrite(ledPin, HIGH); // 点亮 LED
  } else {
    digitalWrite(ledPin, LOW); // 如果不是低电平就关闭 LED
  }
}
