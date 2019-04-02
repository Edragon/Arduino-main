
使用步骤：
1.安装库，将"Arduino Demo_UNO&Mega2560\Install libraries" 路径下的4个库文件夹拷贝至Arduino库目录下（win7系统路径为:C:\Users\Administrator\Documents\Arduino\libraries）；
2.将液晶屏模块对应插入到ArduinoUNO 或者Mega2560开发板上；
3.打开Arduino Demo_UNO&Mega2560文件夹中Example01~Example10共10个示例程序，可分别编译下载进行测试。

常见问题：
1.编译报错。
  请确保所需的库已经全部正确拷贝到系统..\Arduino\libraries相关目录下；
  其次请确认所用的IDE环境是否为1.6.11，其他版本可能会有编译误报错的可能.
2.下载失败。
  开发板类型和串口选择是否正确；

配套示例程序说明:
1.Example01-Simple test为最简单的测试，不需要依赖安装库，可直接刷屏红绿蓝测试模块硬件好坏。
2.Example02~10均需要依赖库，请先将Install libraries里面的库拷贝到Arduino库路径下。
3.扩展例程SDCard Exten Example和Example05-ShowBMP 均需要用到SD卡，涉及SD卡的测试程序仅支持UNO系列，不支持Mega2560.


注意:
本开发资料中涉及的开源Arduino库（MCUFRIEND_kbv,Adafruit_TFTLCD,Adafruit_GFX,TouchScreen）均来自互联网，
我司提供仅作学习用途，版权归原作者所有，如用于商业目的责任自负。

  