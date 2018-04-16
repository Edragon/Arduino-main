#include "lcd.h"
#include "stdlib.h"
#include "usart.h"
#include "delay.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103RBT6,主频72M  单片机工作电压3.3V
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:QDtech2008@gmail.com 
//Skype:QDtech2008
//技术交流QQ群:324828016
//创建日期:2013/5/13
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////	
/****************************************************************************************************
//=========================================电源接线================================================//
//5V接DC 5V电源
//3V3 不用接
//GND接地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为8位并行模式
//8位模式接线：8位模式模块接低8位总线，即:
//液晶屏LCD_D0对应单片机PB0
//液晶屏LCD_D1对应单片机PB1
//液晶屏LCD_D2对应单片机PB2
//液晶屏LCD_D3对应单片机PB3
//液晶屏LCD_D4对应单片机PB4
//液晶屏LCD_D5对应单片机PB5
//液晶屏LCD_D6对应单片机PB6
//液晶屏LCD_D7对应单片机PB7
//=======================================液晶屏控制线接线==========================================//
//LCD_RST	接PC5	//复位信号
//LCD_CS	接PC9	//片选信号
//LCD_RS	接PC8	//寄存器/数据选择信号
//LCD_WR	接PC7	//写信号
//LCD_RD	接PC6	//读信号
//=========================================触摸屏触接线=========================================//
//本程序不支持触摸测试
**************************************************************************************************/	
	   
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 


//******************************************************************
//函数名：  LCD_WR_REG
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    向液晶屏总线写入写16位指令
//输入参数：Reg:待写入的指令值
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WR_REG(u8 data)
{ 
	LCD_RS_CLR;//写地址  
 	LCD_CS_CLR; 
	DATAOUT(data); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET; 	
}

//******************************************************************
//函数名：  LCD_WR_DATA
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    向液晶屏总线写入写16位数据
//输入参数：Data:待写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WR_DATA(u16 data)
{
	LCD_RS_SET;
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;

}
//******************************************************************
//函数名：  LCD_DrawPoint_16Bit
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    8位总线下如何写入一个16位数据
//输入参数：(x,y):光标坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_DrawPoint_16Bit(u16 color)
{
#if LCD_USE8BIT_MODEL==1
	LCD_CS_CLR;
	LCD_RD_SET;
	LCD_RS_SET;//写地址  	 
	DATAOUT(color>>8);	
	LCD_WR_CLR;
	LCD_WR_SET;	
	DATAOUT(color);	
	LCD_WR_CLR;
	LCD_WR_SET;	 
	LCD_CS_SET;
#else
	LCD_WR_DATA(color); 
#endif


}

//******************************************************************
//函数名：  LCD_WriteReg
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    写寄存器数据
//输入参数：LCD_Reg:寄存器地址
//			LCD_RegValue:要写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   
	 
//******************************************************************
//函数名：  LCD_WriteRAM_Prepare
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    开始写GRAM
//			在给液晶屏传送RGB数据前，应该发送写GRAM指令
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

//******************************************************************
//函数名：  LCD_DrawPoint
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    在指定位置写入一个像素点数据
//输入参数：(x,y):光标坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//设置光标位置 
#if LCD_USE8BIT_MODEL==1
	LCD_CS_CLR;
	LCD_RD_SET;
	LCD_RS_SET;//写地址  	 
	DATAOUT(POINT_COLOR>>8);	
	LCD_WR_CLR;
	LCD_WR_SET;	
	DATAOUT(POINT_COLOR);	
	LCD_WR_CLR;
	LCD_WR_SET;	 
	LCD_CS_SET;
#else
	LCD_WR_DATA(POINT_COLOR); 
#endif


}

//******************************************************************
//函数名：  LCD_Clear
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    LCD全屏填充清屏函数
//输入参数：Color:要清屏的填充色
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Clear(u16 Color)
{
	u32 index=0;      
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);	
#if LCD_USE8BIT_MODEL==1
	LCD_RS_SET;//写数据 
	LCD_CS_CLR;	   
	for(index=0;index<lcddev.width*lcddev.height;index++)
	{
		DATAOUT(Color>>8);	
		LCD_WR_CLR;
		LCD_WR_SET;	
		
		DATAOUT(Color);	
		LCD_WR_CLR;
		LCD_WR_SET;	   
	}
	LCD_CS_SET;	
#else //16位模式
	for(index=0;index<lcddev.width*lcddev.height;index++)
	{
		LCD_WR_DATA(Color);		  
	}
#endif
	
} 

//******************************************************************
//函数名：  LCD_GPIOInit
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    液晶屏IO初始化，液晶初始化前要调用此函数
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5;	   //GPIO_Pin_10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC	
	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB
	GPIO_SetBits(GPIOB,GPIO_Pin_All);	 
}

//******************************************************************
//函数名：  LCD_Reset
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    LCD复位函数，液晶初始化前要调用此函数
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(100);	
	LCD_RST_SET;
	delay_ms(50);
}
 	 
//******************************************************************
//函数名：  LCD_Init
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    LCD初始化
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Init(void)
{  
     										 
	LCD_GPIOInit();
 	LCD_RESET();
	
	//************* Start Initial Sequence **********//
	//ILI9486初始化
	LCD_WR_REG(0XF9);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x08);
	
	LCD_WR_REG(0xC0);
	LCD_WR_DATA(0x19);//VREG1OUT POSITIVE
	LCD_WR_DATA(0x1a);//VREG2OUT NEGATIVE
	
	LCD_WR_REG(0xC1);
	LCD_WR_DATA(0x45);//VGH,VGL    VGH>=14V.
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xC2);
	LCD_WR_DATA(0x33);
	
	LCD_WR_REG(0XC5);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x28);//VCM_REG[7:0]. <=0X80.
		
	LCD_WR_REG(0xB1);//OSC Freq set.
	LCD_WR_DATA(0x90);//0xA0=62HZ,0XB0 =70HZ, <=0XB0.
	LCD_WR_DATA(0x11);
	
	LCD_WR_REG(0xB4);
	LCD_WR_DATA(0x02); //2 DOT FRAME MODE,F<=70HZ.
	
	LCD_WR_REG(0xB6);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x42);//0 GS SS SM ISC[3:0];
	LCD_WR_DATA(0x3B);
		
	LCD_WR_REG(0xB7);
	LCD_WR_DATA(0x07);
	
	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0x1F);
	LCD_WR_DATA(0x25);
	LCD_WR_DATA(0x22);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x4E);
	LCD_WR_DATA(0xC6);
	LCD_WR_DATA(0x39);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0XE1);
	LCD_WR_DATA(0x1F);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x1F);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x46);
	LCD_WR_DATA(0x49);
	LCD_WR_DATA(0x31);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x1C);
	LCD_WR_DATA(0x1A);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0XF1);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0xA4);
	LCD_WR_DATA(0x02);
	
	LCD_WR_REG(0XF2);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0xA3);
	LCD_WR_DATA(0x12);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x12);
	LCD_WR_DATA(0xFF);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0XF4);
	LCD_WR_DATA(0x40);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x91);
	LCD_WR_DATA(0x04);
	
	LCD_WR_REG(0XF8);
	LCD_WR_DATA(0x21);
	LCD_WR_DATA(0x04);
	
	LCD_WR_REG(0x36);
	LCD_WR_DATA(0x48);
	
	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x55);
	
	LCD_WR_REG(0x11);
	delay_ms(120);
	//LCD_WR_REG(0x21);
	LCD_WR_REG(0x29);
	
	LCD_SetParam();//设置LCD参数	 
	LCD_LED=1;//点亮背光	 
	LCD_Clear(WHITE);
}
  		  
//******************************************************************
//函数名：  LCD_SetWindows
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    设置显示窗口
//输入参数：(xStar,yStar):窗口左上角起始坐标
//	 	   	(xEnd,yEnd):窗口右下角结束坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	LCD_WriteRAM_Prepare();	//开始写入GRAM			
}   

//******************************************************************
//函数名：  LCD_SetCursor
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    设置光标位置
//输入参数：Xpos:横坐标
//			Ypos:纵坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(Xpos>>8);
	LCD_WR_DATA(0x00FF&Xpos);		
	LCD_WR_DATA(0x0001);
	LCD_WR_DATA(0x003F);
	
	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_DATA(0x00FF&Ypos);		
	LCD_WR_DATA(0x0001);
	LCD_WR_DATA(0x00Df);
	LCD_WriteRAM_Prepare();	//开始写入GRAM		
} 

//******************************************************************
//函数名：  LCD_SetParam
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    设置LCD参数，方便进行横竖屏模式切换
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_SetParam(void)
{ 
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	lcddev.wramcmd=0x2C;
#if USE_HORIZONTAL==1	//使用横屏	  
	lcddev.dir=1;//横屏
	lcddev.width=480;
	lcddev.height=320;			
	LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
#else//竖屏
	lcddev.dir=0;//竖屏				 	 		
	lcddev.width=320;
	lcddev.height=480;	
	LCD_WriteReg(0x36,(1<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
#endif
}	  


void LCD_direction(u8 direction )
{ 
	switch(direction){
		case 3:
			lcddev.width=480;
			lcddev.height=320;
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		case 1:
			lcddev.width=480;
			lcddev.height=320;
			LCD_WriteReg(0x36,(1<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		case 2:						 	 		
			lcddev.width=320;
			lcddev.height=480;	
			LCD_WriteReg(0x36,(1<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 0:						 	 		
			lcddev.width=320;
			lcddev.height=480;	
		LCD_WriteReg(0x36,0x0A);
			//LCD_WriteReg(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		default:break;
	}
		

}	  

