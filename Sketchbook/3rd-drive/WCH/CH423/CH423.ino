#define CH423_SCL  A5
#define CH423_SDA  A4

#define CH423_SDA_SET digitalWrite(CH423_SDA, HIGH)
#define CH423_SCL_SET digitalWrite(CH423_SCL, HIGH)
#define CH423_SDA_CLR digitalWrite(CH423_SDA, LOW)
#define CH423_SCL_CLR digitalWrite(CH423_SCL, LOW)

#define CH423_SCL_D_OUT pinMode(CH423_SCL, OUTPUT)
#define CH423_SDA_D_OUT pinMode(CH423_SDA, OUTPUT)

#define CH423_SCL_D_IN pinMode(CH423_SCL, INPUT)
#define CH423_SDA_D_IN pinMode(CH423_SDA, INPUT)

#define DELAY_0_1US delay(1)

#define CH423_I2C_ADDR1     0x40         // CH423的地址
#define CH423_I2C_MASK      0x3E         // CH423的高字节命令掩码
#define CH423_RD_IO_CMD    0x4D  // 输入I/O引脚当前状态

void setup() {
  
  Serial.begin(9600);
}


void loop() {
  CH423_I2c_Start();
  int DA = CH423_ReadByte();
  CH423_Write(0xff);
  CH423_I2c_Stop();
  
  Serial.println(DA);
  delay(500);
}





void CH423_I2c_Start(  )    // 操作起始
{
  CH423_SDA_SET;    // 发送起始条件的数据信号
  CH423_SDA_D_OUT;    // 设置SDA为输出方向
  CH423_SCL_SET;
  CH423_SCL_D_OUT;    // 设置SCL为输出方向
  delay(1);  // DELAY_0_1US
  CH423_SDA_CLR;    // CH423_SDA_CLR //发送起始信号
  delay(1);
  CH423_SCL_CLR;    // CH423_SCL_CLR //钳住I2C总线，准备发送或接收数据
}

void CH423_I2c_Stop( void )    // 操作结束
{
    CH423_SDA_CLR;
    DELAY_0_1US;
    CH423_SCL_SET;
    DELAY_0_1US;
    CH423_SDA_SET;    // 发送I2C总线结束信号
    DELAY_0_1US;
}


void CH423_I2c_WrByte( unsigned char dat )    // 写一个字节数据
{
    unsigned char i;
    for( i = 0; i != 8; i++ )    // 输出8位数据
    {
        if( dat&0x80 ) { CH423_SDA_SET; }
        else { CH423_SDA_CLR; }
        DELAY_0_1US; 
        CH423_SCL_SET;
        dat<<=1;
        DELAY_0_1US;    // 可选延时
        CH423_SCL_CLR;
    }
    CH423_SDA_SET;
    DELAY_0_1US;
    CH423_SCL_SET;    // 接收应答
    DELAY_0_1US;
    CH423_SCL_CLR;
}


unsigned char CH423_I2c_RdByte( void )    // 读一个字节数据
{
  unsigned char dat, i;
  CH423_SDA_SET;
  CH423_SDA_D_IN;    // 设置SDA为输入方向

  dat = 0;
  for ( i = 0; i != 8; i++ )   // 输入8位数据
  {
    delay(1);    // 可选延时
    CH423_SCL_SET;
    delay(1);  // 可选延时
    dat <<= 1;
    if (CH423_SDA == 1) dat++;   // 输入1位
    CH423_SCL_CLR;
  }
  CH423_SDA_SET;
  delay(1);
  CH423_SCL_SET;    // 发出无效应答
  delay(1);
  CH423_SCL_CLR;
  return (dat);
}

void CH423_Write( unsigned short cmd )    // 写命令
{
    CH423_I2c_Start();    // 启动总线
    CH423_I2c_WrByte( ( ( unsigned char )( cmd>>7 ) & CH423_I2C_MASK ) | CH423_I2C_ADDR1 );
    CH423_I2c_WrByte( ( unsigned char ) cmd );    // 发送数据
    CH423_I2c_Stop();    // 结束总线 
}

void CH423_WriteByte( unsigned short cmd )    // 写出数据
{
    CH423_I2c_Start();    // 启动总线
    CH423_I2c_WrByte( ( unsigned char )( cmd>>8 ) );
    CH423_I2c_WrByte( ( unsigned char ) cmd );    // 发送数据
    CH423_I2c_Stop();    // 结束总线  
}


unsigned char CH423_ReadByte()    // 读取数据
{
    unsigned char din;
    CH423_I2c_Start();    // 启动总线
    CH423_I2c_WrByte( CH423_RD_IO_CMD );    // 此值为0x4D
    din=CH423_I2c_RdByte();    // 读取数据
    CH423_I2c_Stop();    // 结束总线
    return( din );
}
