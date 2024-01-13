// demo: CAN-BUS Shield, send data
#include <mcp_can.h>
#include <SPI.h>



void setup()
{
  Serial.begin(115200);
  // init can bus, baudrate: 100k
  if (CAN.begin(CAN_100KBPS) == CAN_OK) Serial.print("can init ok!!\r\n");
  else Serial.print("Can init fail!!\r\n");
}

unsigned char stmp[8] = {0, 1, 2, 3, 4, 5, 6, 7};
unsigned char stmp2[8] = {8, 7, 6, 5, 4, 3, 2, 1};
void loop()
{
  // send data:  id = 0x00, standrad flame, data len = 8, stmp: data buf
  CAN.sendMsgBuf(0x00, 0, 8, stmp);
  delay(100);                       // send data per 100ms
  CAN.sendMsgBuf(0x00, 0, 8, stmp2);
  delay(100);                       // send data per 100ms
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
