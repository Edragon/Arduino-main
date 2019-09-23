/*------------------------------------------------------------------------------------
 
 * TM1650 nixie tube
 *
 * This example code shows how to use the functions of the TM1650 Library.

-------------------------------------------------------------------------------------*/
/*************************************************************************************/

#include<tm1650.h>

#define ON	DISPLAY_ON
#define OFF	DISPLAY_OFF
#define SegmentMode		_8_SEGMENT_MODE
unsigned char  LightLevel = LV1;							
unsigned char  WorkMode = NORMAL_MODE;

//define each segment
const unsigned char  Seg_test[8]=      {0x20,0x01,0x02,0x04,0x08,0x10,0x40,0x80};	
//number 0-9 code				
const unsigned char Number_arr[10]=   {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};	

unsigned char i,j=0,k=0,l=0,m=0;
unsigned char temp[4]={0x3f,0x06,0x5b,0x4f};
boolean err = 0;

int	pin_SCK = 13;
int	pin_DIO = 12;

TM1650 tm1650(pin_SCK,pin_DIO);

void setup()
{

}

void loop()
{

	//config tm1650
	err = tm1650.DisplayConfig_TM1650(LightLevel,SegmentMode,WorkMode,ON);
	//all nixie tube turn on
	for(i = 0;i < 4; i++)
	{
		err = tm1650.DisplayOneDigi_TM1650(i+1,0xFF);
	}
	delay(500);
		
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
}