#include<tm1650.h>

#define ON	DISPLAY_ON
#define OFF	DISPLAY_OFF
#define SegmentMode		_8_SEGMENT_MODE
unsigned char  LightLevel = LV1;							
unsigned char  WorkMode = NORMAL_MODE;

//define each segment
const unsigned char  Seg_test[8]= {0x20,0x01,0x02,0x04,0x08,0x10,0x40,0x80};	
//number 0-9 code				
const unsigned char Number_arr[10]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};	

unsigned char i,j=0,k=0,l=0,m=0;
unsigned char temp[4]={0x3f,0x06,0x5b,0x4f};
boolean err = 0;
unsigned char key_value=0;
unsigned char show_buff[4];

int	pin_SCK = 13;
int	pin_DIO = 12;

TM1650 tm1650(pin_SCK,pin_DIO);


void setup()
{

}

void loop()
{
	//配置显示模块参数
	err = tm1650.DisplayConfig_TM1650(LightLevel,SegmentMode,WorkMode,ON);

	for(i = 0;i < 4; i++)
	{
		err = tm1650.DisplayOneDigi_TM1650(i+1,0xFF);
	}
	delay(500);
	  for(i = 0;i < 4; i++)
  {
    err = tm1650.DisplayOneDigi_TM1650(i+1,0);
  }
  delay(500);
  
  while(1)
  {
    key_value = tm1650.Scan_Key_TM1650();  //获取按键值
    
/*  
    显示键值
    show_buff[0]=0;
    show_buff[1]=key_value/100;
    show_buff[2]=key_value/10%10;
    show_buff[3]=key_value%10;
*/

    //转换按键值为1 2 3 4
    if(key_value==34){show_buff[3]=1;}
    else if(key_value==38){show_buff[3]=2;}
    else if(key_value==43){show_buff[3]=3;}
    else if(key_value==47){show_buff[3]=4;}
    else{show_buff[3]=0;}

    //显示键值1 2 3 4
    for(i = 0;i < 4; i++)
    {
      err = tm1650. DisplayOneDigi_TM1650(i+1,Number_arr[show_buff[i]]);
    }
  }

/*    
	//loop each segment
	for(m=0;m<=9;m++)
	{
		for(i=0;i<4;i++)
		{
			err = tm1650.DisplayOneDigi_TM1650(i+1,Seg_test[m]);
		}
		delay(500);
	}

	//display form 0 to 9
	for(m=0;m<=9;m++)
	{
		for(i=0;i<4;i++)
		{
			err = tm1650.DisplayOneDigi_TM1650(i+1,Number_arr[m]);
		}
		delay(500);			
	}
	//display the content of array temp[]
	for(i=0;i<4;i++)
	{
		err = tm1650.DisplayOneDigi_TM1650(i+1,temp[i]);
	}
	delay(500);	

	//all turn off
	err = tm1650.DisplayOFF_TM1650(LightLevel,SegmentMode,WorkMode);			
	delay(1000);	
		
	//turn on again
	err = tm1650.DisplayON_TM1650(LightLevel,SegmentMode,WorkMode);			
	delay(1000);	
*/

}
