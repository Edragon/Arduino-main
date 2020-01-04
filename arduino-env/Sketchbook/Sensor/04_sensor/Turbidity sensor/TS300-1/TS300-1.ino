void setup() {
  Serial.begin(9600); //波特率 9600
}
void loop() {
  int sensorValue = analogRead(A0);//读取模拟口 A0 的数据并送给变量
  float voltage = sensorValue * (5.0 / 1024.0); //将模拟读数从 0-1023 转换为电压 0-5V
  Serial.println(voltage); //串口输出电压值
  delay(500);
}
