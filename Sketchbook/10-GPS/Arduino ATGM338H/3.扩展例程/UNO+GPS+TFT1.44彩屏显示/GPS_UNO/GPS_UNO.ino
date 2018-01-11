/******************************************************************
	作者：神秘藏宝室
	店铺：ILoveMCU.taobao.com
	最终解释权归原作者所有，鄙视其他店铺不劳而获的抄袭行为！
******************************************************************/
#include <UTFT.h>


UTFT myGLCD(YYROBOT_TFT144, A2, A1, A5, A4, A3); // Remember to change the model parameter to suit your display module!
//YYROBOT_TFT144  屏幕型号，不用修改
//SDA----A2
//SCL----A1
//CS-----A5
//RST----A4
//RS----A3
//LED---A0  UTFT库里面设定的，如果需要修改需要修改库文件

extern uint8_t SmallFont[];//原始文件在库文件的DefaultFonts.c中
extern uint8_t BigFont[];//原始文件在库文件的DefaultFonts.c中
extern uint8_t SevenSegNumFont[];//原始文件在库文件的DefaultFonts.c中

//此处为了兼容其他的多串口Arduino板子
#define GpsSerial  Serial
#define DebugSerial Serial
int L = 13; //LED指示灯引脚

struct
{
	char GPS_Buffer[80];
	bool isGetData;		//是否获取到GPS数据
	bool isParseData;	//是否解析完成
	char UTCTime[11];		//UTC时间
	char latitude[11];		//纬度
	char N_S[2];		//N/S
	char longitude[12];		//经度
	char E_W[2];		//E/W
	bool isUsefull;		//定位信息是否有效

	char latitudeLast[11];		//纬度
	char longitudeLast[12];		//经度
} Save_Data;

const unsigned int gpsRxBufferLength = 600;
char gpsRxBuffer[gpsRxBufferLength];
unsigned int ii = 0;

bool flagClearTFT = true;


void setup()	//初始化内容
{
	GpsSerial.begin(9600);			//定义波特率9600，和我们店铺的GPS模块输出的波特率一致
	DebugSerial.begin(9600);
	DebugSerial.println("ILoveMCU.taobao.com");
	DebugSerial.println("Wating...");

	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;


	randomSeed(analogRead(0));
// Setup the LCD
	myGLCD.InitLCD();//初始化液晶
	myGLCD.InitLCD();//初始化两次有利于系统稳定
	myGLCD.InitLCD(PORTRAIT);//竖向显示
	myGLCD.setFont(SmallFont);//设置字体为SmallFont格式
	myGLCD.setColor(255, 255, 255);//设置字体颜色
	myGLCD.setBackColor(255, 0, 0);//设置背景颜色
	myGLCD.clrScr();

	myGLCD.print("GPS Initialize", LEFT, 20);
	myGLCD.print("waiting...", LEFT, 35);
	myGLCD.print("...", LEFT, 50);
	myGLCD.print("ilovemcu.taobao", LEFT, 65);
	myGLCD.print(".com", RIGHT, 80);
}

void loop()		//主循环
{
	gpsRead();	//获取GPS数据
	parseGpsBuffer();//解析GPS数据
	printGpsBuffer();//输出串口解析后的数据并TFT显示
	// DebugSerial.println("\r\n\r\nloop\r\n\r\n");


}

void displayGpsData()
{
	if(flagClearTFT)	//切换屏幕要清屏,第一次显示的内容再这里显示
	{
		flagClearTFT = false;
		myGLCD.clrScr();
		myGLCD.print("UTCTime:", LEFT, 10);
		myGLCD.print("latitude:", LEFT, 50);
		myGLCD.print(Save_Data.N_S, RIGHT, 65);
		myGLCD.print("longitude", LEFT, 90);
		myGLCD.print(Save_Data.E_W, RIGHT, 105);	
	}

	myGLCD.print(Save_Data.UTCTime, LEFT, 25);

	int res = strcmp(Save_Data.latitude,Save_Data.latitudeLast); 
	//比较新数据和上次的数据是否相同
	//因为TFT刷新速度太慢，为防止刷屏时候GPS数据丢失或者串口缓存溢出，采取方法：只有数据改变才刷屏 
	if(res != 0)
	{	
		myGLCD.print(Save_Data.latitude, LEFT, 65);	
	}
	
	res = strcmp(Save_Data.longitude,Save_Data.longitudeLast);  
	if(res != 0)
	{
		myGLCD.print(Save_Data.longitude, LEFT, 105);		
	}





}

void errorLog(int num)
{
	DebugSerial.print("ERROR");
	DebugSerial.println(num);
	while (1)
	{
		digitalWrite(L, HIGH);
		delay(300);
		digitalWrite(L, LOW);
		delay(300);
	}
}

void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;

		DebugSerial.print("Save_Data.UTCTime = ");
		DebugSerial.println(Save_Data.UTCTime);

		if (Save_Data.isUsefull)
		{
			displayGpsData();

			Save_Data.isUsefull = false;
			DebugSerial.print("Save_Data.latitudeLast = ");
			DebugSerial.println(Save_Data.latitudeLast);
			DebugSerial.print("Save_Data.latitude = ");
			DebugSerial.println(Save_Data.latitude);
			DebugSerial.print("Save_Data.N_S = ");
			DebugSerial.println(Save_Data.N_S);

			DebugSerial.print("Save_Data.longitudeLast = ");
			DebugSerial.println(Save_Data.longitudeLast);
			DebugSerial.print("Save_Data.longitude = ");
			DebugSerial.println(Save_Data.longitude);
			DebugSerial.print("Save_Data.E_W = ");
			DebugSerial.println(Save_Data.E_W);
		}
		else
		{
			DebugSerial.println("GPS DATA is not usefull!");
		}

	}
}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		DebugSerial.println("**************");
		DebugSerial.println(Save_Data.GPS_Buffer);

		memcpy(Save_Data.latitudeLast, Save_Data.latitude, 11);	//保存上一次的经纬度数据，为了是否有变化
		memcpy(Save_Data.longitudeLast, Save_Data.longitude, 12);
		for (int i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2];
					switch (i)
					{
					case 1: memcpy(Save_Data.UTCTime, subString, subStringNext - subString); break;	//获取UTC时间
					case 2: memcpy(usefullBuffer, subString, subStringNext - subString); break;	//获取UTC时间
					case 3: memcpy(Save_Data.latitude, subString, subStringNext - subString); break;	//获取纬度信息
					case 4: memcpy(Save_Data.N_S, subString, subStringNext - subString); break;	//获取N/S
					case 5: memcpy(Save_Data.longitude, subString, subStringNext - subString); break;	//获取纬度信息
					case 6: memcpy(Save_Data.E_W, subString, subStringNext - subString); break;	//获取E/W

					default: break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if (usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if (usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//解析错误
				}
			}


		}
	}
}


void gpsRead() {
	while (GpsSerial.available())
	{
		gpsRxBuffer[ii++] = GpsSerial.read();
		if (ii == gpsRxBufferLength)clrGpsRxBuffer();
	}

	char* GPS_BufferHead;
	char* GPS_BufferTail;
	if ((GPS_BufferHead = strstr(gpsRxBuffer, "$GPRMC,")) != NULL || (GPS_BufferHead = strstr(gpsRxBuffer, "$GNRMC,")) != NULL )
	{
		if (((GPS_BufferTail = strstr(GPS_BufferHead, "\r\n")) != NULL) && (GPS_BufferTail > GPS_BufferHead))
		{
			memcpy(Save_Data.GPS_Buffer, GPS_BufferHead, GPS_BufferTail - GPS_BufferHead);
			Save_Data.isGetData = true;

			clrGpsRxBuffer();
		}
	}
}

void clrGpsRxBuffer(void)
{
	memset(gpsRxBuffer, 0, gpsRxBufferLength);      //清空
	ii = 0;
}

