#include <Wire.h>
union data {
  int16_t data16;
  byte  byteStr[2];//{*DATAL,*DATAH}
};
union data xData;
union data yData;
union data zData;

const int sendorDeviceAddress = 0x1F;//I2C7bitAddressMode
const int regAddressXOUTL = 0x06;
const int regAddressYOUTL = 0x08;
const int regAddressZOUTL = 0x0A;
const int regAddressODCNTL = 0x1B;
const int regAddressCNTL1 = 0x18;


void setup() {
  Serial.begin(115200);


  //KX023debice initialize
  Wire.begin();
  //stunbymode fullPower GLange2g
  Wire.beginTransmission(sendorDeviceAddress);
  Wire.write(0x18);
  Wire.write(0x40);
  Wire.endTransmission();
  //outPutDataLate 50Hz
  Wire.beginTransmission(sendorDeviceAddress);
  Wire.write(0x1B);
  Wire.write(0x02);
  Wire.endTransmission();
  //sensor WakeUp
  Wire.beginTransmission(sendorDeviceAddress);
  Wire.write(0x18);
  Wire.write(0xC0);
  Wire.endTransmission();
  
  //setup LPF--------------------------------------------------
  //ここを初期化処理に追加すると加速度センサ側でローパスフィルタがかかります
  
  //set device standbyMode
  //readCNTL1reg
  byte CNTL1 = 0;
  Wire.beginTransmission(sendorDeviceAddress);
  Wire.write(regAddressCNTL1);
  Wire.endTransmission();
  Wire.requestFrom(sendorDeviceAddress, 1);
  CNTL1 = Wire.read();
  //setCNTL1reg
  CNTL1 = CNTL1 & 0b01111111;
  Wire.beginTransmission(sendorDeviceAddress);
  Wire.write(regAddressCNTL1);
  Wire.write(CNTL1);
  Wire.endTransmission();

  //set LPF parameters
  //readODCNTLreg
  byte ODCNTL = 0;
  Wire.beginTransmission(sendorDeviceAddress);
  Wire.write(regAddressODCNTL);
  Wire.endTransmission();
  Wire.requestFrom(sendorDeviceAddress, 1);
  ODCNTL = Wire.read();
  //setODCNTLreg
  ODCNTL = ODCNTL | 0b01000000;//set filter corner frequency set to ODR/2
  ODCNTL = ODCNTL & 0b11110000;//set OutputDataRate 12.5Hz
  Wire.beginTransmission(sendorDeviceAddress);
  Wire.write(regAddressODCNTL);
  Wire.write(ODCNTL);
  Wire.endTransmission();
  
  //set device operating mode
  //readCNTL1reg
  Wire.beginTransmission(sendorDeviceAddress);
  Wire.write(regAddressCNTL1);
  Wire.endTransmission();
  Wire.requestFrom(sendorDeviceAddress, 1);
  CNTL1 = Wire.read();
  //setCNTL1reg
  CNTL1 = CNTL1 | 0b10000000;
  Wire.beginTransmission(sendorDeviceAddress);
  Wire.write(regAddressCNTL1);
  Wire.write(CNTL1);
  Wire.endTransmission();
  //--------------------------------------------------setup LPF

}

void loop() {

  //readXout
  Wire.beginTransmission(sendorDeviceAddress);
  Wire.write(regAddressXOUTL);
  Wire.endTransmission();
  Wire.requestFrom(sendorDeviceAddress, 2);
  xData.byteStr[0] = Wire.read();
  xData.byteStr[1] = Wire.read();

  //readYout
  Wire.beginTransmission(sendorDeviceAddress);
  Wire.write(regAddressYOUTL);
  Wire.endTransmission();
  Wire.requestFrom(sendorDeviceAddress, 2);
  yData.byteStr[0] = Wire.read();
  yData.byteStr[1] = Wire.read();

  //readZout
  Wire.beginTransmission(sendorDeviceAddress);
  Wire.write(regAddressZOUTL);
  Wire.endTransmission();
  Wire.requestFrom(sendorDeviceAddress, 2);
  zData.byteStr[0] = Wire.read();
  zData.byteStr[1] = Wire.read();

//  //rawDataOutput
//  Serial.print("xdata:");
//  Serial.print(xData.data16, DEC);
//  Serial.print(",");
//  Serial.print("ydata:");
//  Serial.print(yData.data16, DEC);
//  Serial.print(",");
//  Serial.print("zdata:");
//  Serial.println(zData.data16, DEC);

  Serial.print(xData.data16, DEC);
  Serial.print(",");
  Serial.print(yData.data16, DEC);
  Serial.print(",");
  Serial.println(zData.data16, DEC);

//long xdata = xData.data16;
//long ydata = yData.data16;
//long zdata = zData.data16;
//
//double Norm = sqrt(double(pow(xdata,2) + pow(ydata,2) + pow(zdata,2)))-(32767/2);
//Serial.print(Norm,DEC);
//Serial.print(',');
//static double vector = vector+Norm;
//Serial.println(vector,DEC);

  delay(100);

}
