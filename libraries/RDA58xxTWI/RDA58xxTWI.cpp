#include <compat/twi.h>

#define uchar unsigned char
#define uint unsigned int
#include "RDA58xxTWI.h"

#define Wr_IIC 0x22 //RDA58xx芯片的I2C地址！
#define Rd_IIC (Wr_IIC +1)  //因读写时序问题，RDA58xx的读地址比写地址+1.


/*-----初始化TWI总线函数TWI-INI ----*/

void twi_ini() 
{ 
    TWCR=0X00; //关闭TWI总线 
    TWBR=10;   //设置传输比特率  
    TWSR=0;    //设置TWI总线的速度，
    TWCR=0x04; //使能TWI总线 
} 


/*-----RDA58XX写入数据函数 WRI_58xx() --------*/
uchar WriteTo_58xx(uchar REG_ADDRESS,uint SEND_DATA)
{

uchar TxH,TxL; //////////分解为输入的数据为高8位和低8位。
TxL=SEND_DATA;			
SEND_DATA>>=8;
TxH=SEND_DATA;

twi_ini() ;        //初始化TWI总线

TWI_START;
TWI_WAIT; 						//等待START发出:正在发时TWINT为0，傻等他发好了TWINT为1
TWI_TEST_ACK;

TWI_SEND_BYTE(Wr_IIC); 			//再次置位这两个，就是开始发出写，5807器件地址0X22
TWI_WAIT; 						//等待START发出:正在发时TWINT为0，发好了TWINT为1;

TWI_SEND_BYTE(REG_ADDRESS); 	//写寄存器地址
TWI_WAIT; ; 					//等待发送完成及给出应答信号

TWI_SEND_BYTE(TxH); 			//写高字节
TWI_WAIT; 						//等待数据发送完成及给出应答信号

TWI_SEND_BYTE(TxL); 			//写低字节
TWI_WAIT; 						//等待数据发送完成及给出应答信号

TWI_STOP;

}
/*-----RDA58XX读取数据函数 WRI_58xx() --------*/
uint ReadFrom_58xx(uchar REG_ADDRESS)
{

twi_ini() ;        //初始化TWI总线

uint Read_Buff;
uchar RxH,RxL;
TWI_START;
TWI_WAIT; //等待START发出:正在发时TWINT为0，发好了TWINT为1

TWI_SEND_BYTE(Wr_IIC); //发出写，5807器件地址0X22
TWI_WAIT;

TWI_SEND_BYTE(REG_ADDRESS); //发出写，寄存器地址0X0A或0X0B
TWI_WAIT;

TWI_START; //再启动
TWI_WAIT;

TWI_SEND_BYTE(Rd_IIC); //读5807器件地址0X23
TWI_WAIT;

TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);//接收数据后发出应答：1<<TWEA
TWI_WAIT;

RxH=TWDR; //读出寄存器高字节
TWCR&=~(1<<TWEA);
TWCR=(1<<TWINT)|(1<<TWEN);

TWI_WAIT;

RxL=TWDR; //读出寄存器低字节
TWI_STOP;


Read_Buff=0;
Read_Buff+=RxH;
Read_Buff<<=8;
Read_Buff+=RxL;
return Read_Buff; //返回状态字
}
