/*------------------------------------------------------------------------------------

  TM1650.h - Library for  nixie tube driving and Keypad scan IC TM1668， but 
  doesn't  include the keypad scan function yet.
  Created by Junjie Tong From China, March 12, 2017.
  Released into the public domain.

-------------------------------------------------------------------------------------*/

/************************包含头文件***************************************************/

#include<Arduino.h>
#include<TM1650.h>

const unsigned char DisplayAddressArray[4] = {DIG1_ADDRESS,DIG2_ADDRESS,DIG3_ADDRESS,DIG4_ADDRESS};

/**************************************************************************************/	
TM1650::TM1650(int pin_SCK,int pin_DIO)
{
	pinMode(pin_SCK, OUTPUT);
	pinMode(pin_DIO, OUTPUT);
	
	_pin_SCK = pin_SCK;
	_pin_DIO = pin_DIO;
}
/**************************************************************************************/
void TM1650::FrameStart_1650(void)
{

	digitalWrite(_pin_DIO , HIGH);
	digitalWrite(_pin_SCK , HIGH);		  
	digitalWrite(_pin_DIO , LOW);
}

/**************************************************************************************/
void TM1650::FrameEnd_1650(void)
{
	digitalWrite(_pin_DIO , LOW);
	digitalWrite(_pin_SCK , HIGH);		  
	digitalWrite(_pin_DIO , HIGH);
}
/**************************************************************************************/
boolean TM1650::FrameAck_1650(void)
{
	//pinMode(_pin_DIO, INPUT);
	if(digitalRead(_pin_DIO) == LOW)
	{
		digitalWrite(_pin_SCK , HIGH);	
		digitalWrite(_pin_SCK , LOW);	
		return 0;
	}
	else
	{
		return 1;
	}
	//pinMode(_pin_DIO, OUTPUT);
}
/**************************************************************************************/
boolean TM1650::SendOne_TM1650(unsigned char firstByte)
{
	unsigned char tmp;
	unsigned char i=0;
	boolean err=0;

	tmp=firstByte;
	FrameStart_1650();

	for(i=0;i<8;i++)
	{
		if(tmp&0x80)
		{
			digitalWrite(_pin_DIO , HIGH);
		}
		else
		{
			digitalWrite(_pin_DIO , LOW);
		}
	
		digitalWrite(_pin_SCK , LOW);
		digitalWrite(_pin_SCK , HIGH);
		digitalWrite(_pin_SCK , LOW);
		
		tmp=tmp<<1;
	}
	if(FrameAck_1650()==1)
	{
		err=1;
	}
	return err;
}
/**************************************************************************************/
boolean TM1650::SendOneCouple_TM1650(unsigned char firstByte,unsigned char secondByte)
{
	unsigned char tmp;
	unsigned char i=0;
	boolean err=0;		

	tmp=firstByte;

	FrameStart_1650();

	for(i=0;i<8;i++)
	{
		if(tmp&0x80)
		{
			digitalWrite(_pin_DIO , HIGH);
		}
		else
		{
			digitalWrite(_pin_DIO , LOW);
		}
	
		digitalWrite(_pin_SCK , LOW);
		digitalWrite(_pin_SCK , HIGH);
		digitalWrite(_pin_SCK , LOW);
		
		tmp=tmp<<1;
	}

	if(FrameAck_1650()==1)
	{
		err=1;
	}

	tmp=secondByte;
	for(i=0;i<8;i++)
	{
		if(tmp&0x80)
		{
			digitalWrite(_pin_DIO , HIGH);
		}
		else
		{
			digitalWrite(_pin_DIO , LOW);
		}
	
		digitalWrite(_pin_SCK , LOW);
		digitalWrite(_pin_SCK , HIGH);
		digitalWrite(_pin_SCK , LOW);
		
		tmp=tmp<<1;
	}

	if(FrameAck_1650()==1)
	{
		err=1;
	}

	FrameEnd_1650();

	return err;
}
/**************************************************************************************/
boolean TM1650::DisplayConfig_TM1650(unsigned char lightLevel,unsigned char SegmentMode,unsigned char WorkMode,unsigned char on_or_off)
{
	unsigned char tmp = 0;
	boolean err = 0;
	tmp = lightLevel | SegmentMode |  WorkMode | on_or_off;
	err = SendOneCouple_TM1650(CMD_SYSTEM_CONFIG,tmp);

	if(err == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
/**************************************************************************************/
boolean TM1650::DisplayOneDigi_TM1650(unsigned char digi,unsigned char cha)
{
	boolean err = 0;
	err = SendOneCouple_TM1650(DisplayAddressArray[digi-1],cha);
	return err;
}

/**************************************************************************************/
boolean TM1650::DisplayOFF_TM1650(unsigned char lightLevel,unsigned char SegmentMode,unsigned char WorkMode)
{
	unsigned char tmp = 0;
	boolean err = 0;
	tmp = lightLevel | SegmentMode |  WorkMode | DISPLAY_OFF;
	err = SendOneCouple_TM1650(CMD_SYSTEM_CONFIG,tmp);

	if(err == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**************************************************************************************/
boolean TM1650::DisplayON_TM1650(unsigned char lightLevel,unsigned char SegmentMode,unsigned char WorkMode)
{
	unsigned char tmp = 0;
	boolean err = 0;
	tmp = lightLevel | SegmentMode |  WorkMode | DISPLAY_ON;
	err = SendOneCouple_TM1650(CMD_SYSTEM_CONFIG,tmp);

	if(err == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**************************************************************************************/
unsigned char TM1650::Scan_Key_TM1650()
	{
        unsigned char i;
        unsigned char rekey=0;
        FrameStart_1650();
				SendOne_TM1650(CMD_READ_KEYPAD);
        FrameAck_1650();
        pinMode(_pin_DIO, INPUT);
        for(i=0;i<8;i++)
        {
           digitalWrite(_pin_SCK , HIGH);
           rekey = rekey<<1;
           if(digitalRead(_pin_DIO))
           {
                   rekey++;
           } 
           digitalWrite(_pin_SCK , LOW);        
        }
        pinMode(_pin_DIO, OUTPUT);
        FrameAck_1650();
        FrameEnd_1650();
        return(rekey);
}

/***************************************END******************************************/
