#include "RDA58xxTWI.h"//RDA5820头文件
#include <Wire.h>//I2C函数库

void setup() {
  Serial.begin(9600);
  initRda5820();//初始化FM模块主要上电复位和设置一些其他功能
  //RX_Fm(10710);//FM接收模式
  // TX_Fm();//FM发送模式
}

void loop() {
    RX_Fm(10710);// 中国国际广播
    Serial.print("FMtx=");
    Serial.print(Rda5820FreqGet());//FM频率
    Serial.println(" ");
    delay(50000);


  //  RX_Fm(9910);// 龙华电视台
  //  Serial.print("FMtx=");
  //  Serial.print(Rda5820FreqGet());//FM频率
  //  Serial.println(" ");
  //  delay(5000);

  RX_Fm(8980);// 宝安电视台 // 898
  Serial.print("FMtx=");
  Serial.print(Rda5820FreqGet());//FM频率
  Serial.println(" ");

  //  RX_Fm(10620);// 宝安电视台 // 10430
  //  Serial.print("FMtx=");
  //  Serial.print(Rda5820FreqGet());//FM频率
  //  Serial.println(" ");
  //  delay(50000);
}

void initRda5820()//FM初始化
{
  WriteTo_58xx(0x02, 0x0002); //芯片软复位
  delay(400);//等待复位结束
  WriteTo_58xx(0x02, 0x0001); //芯片上电
  delay(800);//等待复位结束
  WriteTo_58xx(0x02, 0xD201); //0C201芯片上电,不复位 正常天线 32.768时钟 循环搜索 不开始搜索 想上搜索 低音增强 立体声 非静音 非高阻抗
}
