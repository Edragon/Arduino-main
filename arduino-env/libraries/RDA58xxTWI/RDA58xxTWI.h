////////////	RDA58xxTWI.h  /////////////
/*
此库根据卫小鲁先生的程序修改而成。在此对卫先生致以衷心的感谢！此库为arduino开发使用，遵循arduino的开源政策。
此库针对RDA58系列芯片I2C总线读写操作设计。如何使用芯片寄存器，请参考芯片的数据手册。
*/
#define RDA58xxTWI.h




#define TWI_START  TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN)
#define TWI_STOP   TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN)
#define TWI_WAIT   {while(!(TWCR&(1<<TWINT)));}
#define TWI_TEST_ACK TWSR&0XF8
#define TWI_SEND_ACK  TWCR|=(1<<TWEA)
#define TWI_SEND_BYTE(X) {TWDR=(X);TWCR=(1<<TWINT)|(1<<TWEN);}
#define uchar unsigned char
#define uint unsigned int

void twi_ini();
uchar WriteTo_58xx(uchar REG_ADDRESS,uint SEND_DATA);
uint ReadFrom_58xx(uchar REG_ADDRESS);
//==================================================================
/////////////////////////////////////////////////////////


