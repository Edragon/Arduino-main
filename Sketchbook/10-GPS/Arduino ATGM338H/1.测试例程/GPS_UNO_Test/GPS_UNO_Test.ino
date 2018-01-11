//此处为了兼容其他的多串口Arduino板子
#define GPSSerial  Serial
#define DEBUGSerial Serial


void setup()	//初始化内容
{
  GPSSerial.begin(9600);			//定义波特率9600，和我们店铺的GPS模块输出的波特率一致
  DEBUGSerial.begin(9600);  
  DEBUGSerial.println("ILoveMCU.taobao.com");
  DEBUGSerial.println("Wating...");
}

void loop()		//主循环
{
  while (GPSSerial.available()) {   
     DEBUGSerial.write(GPSSerial.read());//收到GPS数据则通过Serial输出
  }
  

}

