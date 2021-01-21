// demo: CAN-BUS Shield, receive data
#include <mcp_can.h>
#include <SPI.h>

unsigned char Flag_Recv = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];

void setup()
{
  Serial.begin(115200);
  
  if (CAN.begin(CAN_500KBPS) == CAN_OK) Serial.print("can init ok!!\r\n");
  else Serial.print("Can init fail!!\r\n"); 
  
  //attachInterrupt(0, MCP2515_ISR, FALLING);    // start interrupt
  
}

void MCP2515_ISR()
{
  Flag_Recv = 1;
}

void loop()
{
  //if (Flag_Recv)                          // check if get data
  //{
  //  Flag_Recv = 0;                        // clear flag
    CAN.readMsgBuf(&len, buf);            // read data,  len: data length, buf: data buf
    Serial.println("CAN_BUS GET DATA!");
    Serial.print("data len = ");
    Serial.println(len);

    for (int i = 0; i < len; i++)         // print the data
    {
      Serial.print(buf[i]); Serial.print("\t");
    }
    Serial.println();
  //}
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
