#include "RDA58xxTWI.h"
#include <Wire.h>
//以下程序在RDA5820测试通过

#define Wr_IIC 0x22 //rda5807的地址！
#define Rd_IIC (Wr_IIC + 1 )

void setup() {
  unsigned int CHAN,VALUE;
    Serial.begin(9600); 
    WriteTo_58xx(0x40,0x0000);    //设置芯片为接收模式
    WriteTo_58xx(0x02,0b1101001000000101);   //设置芯片工作状态
    CHAN = 870;                 //设置开始搜台频率870Mhz，这里在上面已经设置了芯片接收频段87MHz-108MHz
    CHAN = CHAN - 870 ;
    VALUE = CHAN << 6 ;
    VALUE += 0X0000; 
    WriteTo_58xx(0x03,VALUE);       //设置频率。
    WriteTo_58xx(0x02,0b1101001100000101); //开始搜台
    WriteTo_58xx(0x05,0b1000100000011111); //设置音量最大
	delay(500);
}
unsigned int reading;
void loop() {
    Serial.println("Hi!FM Tune");
    delay(500);
    unsigned int tmp;
    char tmph,tmpl;
    reading=ReadFrom_58xx(0x0a);
      if (2<=reading){
        reading<<=6;
        reading>>=6;
        float addfm = reading;
        float showfm = (870 + addfm ) / 10;
        Serial.print("FM:");
        Serial.print( showfm);
        Serial.println("MHz");
      
    }
      else {
        Serial.println("no date!");
        }


  Serial.println(tmp);
	delay(500);

}
