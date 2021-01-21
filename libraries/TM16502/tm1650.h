/*-------------------------------------------------------------------------------------

  TM1650.h - Library for  nixie tube driving and Keypad scan IC TM1668£¬ but 
  doesn't  include the keypad scan function yet.
  Created by Junjie Tong From China, March 12, 2017.
  Released into the public domain.


----------------------------------------------------------------------------------------*/

#ifndef _TM1650_H_
#define _TM1650_H_

#include <Arduino.h>

/******************************Definitions********************************************/
#define LV1	0x00
#define LV2	0x10
#define LV3	0x20
#define LV4	0x30
#define LV5	0x40
#define LV6	0x50
#define LV7	0x60
#define LV8	0x70

#define _8_SEGMENT_MODE	0x00	   
#define _7_SEGMENT_MODE	0x08	  

#define NORMAL_MODE		0x00	  
#define STANDBY_MODE	0x04	  

#define DISPLAY_ON		0x01	   
#define DISPLAY_OFF		0x00	  

#define CMD_SYSTEM_CONFIG	0x48   
#define CMD_READ_KEYPAD		0x4F   

#define DIG1_ADDRESS	0x68	  
#define DIG2_ADDRESS    0x6A	   
#define DIG3_ADDRESS	0x6C	   
#define DIG4_ADDRESS    0x6E	  

 /*************************************************************************************/
class TM1650
{
	public:
		TM1650(int pin_SCK,int pin_DIO);
		boolean DisplayConfig_TM1650(unsigned char lightLevel,unsigned char SegmentMode,unsigned char WorkMode,unsigned char on_or_off);
		boolean DisplayOneDigi_TM1650(unsigned char digi,unsigned char cha);
		boolean DisplayOFF_TM1650(unsigned char lightLevel,unsigned char SegmentMode,unsigned char WorkMode);
		boolean DisplayON_TM1650(unsigned char lightLevel,unsigned char SegmentMode,unsigned char WorkMode);
		unsigned char Scan_Key_TM1650();
	
	private:
		int _pin_SCK;			//clock in pin
		int _pin_DIO;			//data in and out pin
		
		void FrameStart_1650(void);
		void FrameEnd_1650(void);
		boolean FrameAck_1650(void);
		boolean SendOneCouple_TM1650(unsigned char firstByte,unsigned char secondByte);
		boolean SendOne_TM1650(unsigned char firstByte);
};	
#endif