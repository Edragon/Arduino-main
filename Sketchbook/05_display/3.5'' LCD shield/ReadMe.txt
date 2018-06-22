【EN】
Use steps:
1. Install the library and copy the four library folders under the path of "Arduino Demo_UNO&Mega2560\Install libraries" to the Arduino library directory.
2. Insert the LCD module into the ArduinoUNO or Mega2560 development board;
3. Open 12 sample programs in the Arduino Demo_UNO&Mega2560 folder with Example01~ Example12, which can be compiled and downloaded separately.

sample program description:
1. All Examples will need to rely on the library. Please copy the library in Install libraries to the Arduino library path.
2.Example09~Example11 test touch function, if your module is without touch function, the operation of this program cannot be touched only to see the display.
3. SDCard Exten Example and example08-showbmp all need SD card, the test program involving SD card only supports UNO series, not Mega2560.
Note:
This development materials involved in the open source Arduino libraries (MCUFRIEND_kbv Adafruit_TFTLCD, Adafruit_GFX, TouchScreen) are from the Internet, only can be used as a study and communication purposes, all peer originator. Please contact the original author for commercial purposes, otherwise the responsibility will be overdone.

Common problems:
1. Compilation of errors.
Please ensure that the required libraries are fully copied to the system. \Arduino\libraries related directories;
Secondly, please confirm whether the IDE environment used is 1.6.11 or more, and other versions may have the possibility of compiling false errors.
2. Failure to download.
Development board type and serial port selection are correct;



【CN】
使用步骤：
1.安装库，将"Arduino Demo_UNO&Mega2560\Install libraries" 路径下的4个库文件夹拷贝至Arduino库目录下（win7系统一般默认路径为:C:\Users\Administrator\Documents\Arduino\libraries）；
2.将液晶屏模块对应插入到ArduinoUNO 或者Mega2560开发板上；
3.打开Arduino Demo_UNO&Mega2560文件夹中Example01~Example12共12个示例程序，可分别编译下载进行测试。

常见问题：
1.编译报错。
  请确保所需的库已经全部正确拷贝到系统..\Arduino\libraries相关目录下；
  其次请确认所用的IDE环境是否为1.6.11以上版本，其他版本可能会有编译误报错的可能.
2.下载失败。
  开发板类型和串口选择是否正确；

配套示例程序说明:
1.所有Examples均需要依赖库，请先将Install libraries里面的库拷贝到Arduino库路径下。
2.Example09~Example11测试触摸功能，如果你的模块是不带触摸功能，则运行此程序是无法进行触摸的只能看到显示。
3.扩展例程SDCard Exten Example和Example08-ShowBMP 均需要用到SD卡，涉及SD卡的测试程序仅支持UNO系列，不支持Mega2560.


注意:
本开发资料中涉及的开源Arduino库（MCUFRIEND_kbv,Adafruit_TFTLCD,Adafruit_GFX,TouchScreen）均来自互联网，仅可用作学习交流用途，版权归原作者所有。如用于商业目的请联系原作者，否则责任自负。



  