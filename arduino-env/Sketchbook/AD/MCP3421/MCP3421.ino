#include <Wire.h>

// I2C address for MCP3422 - base address for MCP3424
#define MCP3422_ADDRESS 0X68

// fields in configuration register
#define MCP342X_GAIN_FIELD 0X03 // PGA field
#define MCP342X_GAIN_X1    0X00 // PGA gain X1
#define MCP342X_GAIN_X2    0X01 // PGA gain X2
#define MCP342X_GAIN_X4    0X02 // PGA gain X4
#define MCP342X_GAIN_X8    0X03 // PGA gain X8

#define MCP342X_RES_FIELD  0X0C // resolution/rate field
#define MCP342X_RES_SHIFT  2    // shift to low bits
#define MCP342X_12_BIT     0X00 // 12-bit 240 SPS
#define MCP342X_14_BIT     0X04 // 14-bit 60 SPS
#define MCP342X_16_BIT     0X08 // 16-bit 15 SPS
#define MCP342X_18_BIT     0X0C // 18-bit 3.75 SPS

#define MCP342X_CONTINUOUS 0X10 // 1 = continuous, 0 = one-shot

#define MCP342X_CHAN_FIELD 0X60 // channel field
#define MCP342X_CHANNEL_1  0X00 // select MUX channel 1
#define MCP342X_CHANNEL_2  0X20 // select MUX channel 2
#define MCP342X_CHANNEL_3  0X40 // select MUX channel 3
#define MCP342X_CHANNEL_4  0X60 // select MUX channel 4

#define MCP342X_START      0X80 // write: start a conversion
#define MCP342X_BUSY       0X80 // read: output not ready

//------------------------------------------------------------------------
// default adc congifuration register - resolution and gain added in setup()
uint8_t adcConfig = MCP342X_START | MCP342X_CHANNEL_1 | MCP342X_CONTINUOUS;
// divisor to convert ADC reading to milivolts
uint16_t mvDivisor;
//------------------------------------------------------------------------------
void halt(void) 
{
  Serial.println("Halted");
  while(1);
}
//------------------------------------------------------------------------------
// read mcp342x data
uint8_t mcp342xRead(int32_t &data)
{
  // pointer used to form int32 data
  uint8_t *p = (uint8_t *)&data;
  // timeout - not really needed?
  uint32_t start = millis();
  do {
    // assume 18-bit mode
    Wire.requestFrom(MCP3422_ADDRESS, 4);
    if (Wire.available() != 4) {
      Serial.println("read failed");
      return false;
    }
    for (int8_t i = 2; i >= 0; i--) {
      p[i] = Wire.read();
    }
    // extend sign bits
    p[3] = p[2] & 0X80 ? 0XFF : 0;
    // read config/status byte
    uint8_t s = Wire.read();
    if ((s & MCP342X_RES_FIELD) != MCP342X_18_BIT) {
      // not 18 bits - shift bytes for 12, 14, or 16 bits
      p[0] = p[1];
      p[1] = p[2];
      p[2] = p[3];
    }
    if ((s & MCP342X_BUSY) == 0) return true;
  } while (millis() - start < 500); //allows rollover of millis()
  Serial.println("read timeout"); 
  return false;
}
//------------------------------------------------------------------------------
// write mcp342x configuration byte
uint8_t mcp342xWrite(uint8_t config)
{
  Wire.beginTransmission(MCP3422_ADDRESS);
  Wire.write(config); 
  Wire.endTransmission();
}

//------------------------------------------------------------------------------
void setup()
{
  Serial.begin(9600);  
  Wire.begin();
  uint8_t chan = 0XFF, gain = 0XFF, res = 0XFF;
  do {
    Serial.flush();
    Serial.print("\nEnter channel (1, 2, 3, 4): ");
    while (Serial.available() == 0);
    chan = Serial.read() - '1';
  } while(chan > 3);
  Serial.println(chan + 1, DEC); 
  do {
    Serial.println();
    Serial.flush();
    Serial.print("Enter gain (1, 2, 4, or 8): ");
    while(Serial.available() < 1);
    switch (Serial.read()) {
      case '1': gain = 0; break;
      case '2': gain = 1; break;
      case '4': gain = 2; break;
      case '8': gain = 3; break;
    }
  } while (gain > 3);
  Serial.println(1 << gain, DEC);
  do {
    Serial.flush();
    Serial.println();
    Serial.print("Enter resolution (12, 14, 16, or 18): ");
    while (Serial.available() < 2);
    if (Serial.read() != '1') continue;
    switch (Serial.read()) {
      case '2': res = 0; break;
      case '4': res = 1; break;
      case '6': res = 2; break;
      case '8': res = 3; break;
    }
  } while (res > 3);
  Serial.println(12 + 2*res, DEC);

  adcConfig |= chan << 5 | res << 2 | gain;
  // divisor to convert ADC reading to millivolts
  mvDivisor = 1 << (gain + 2*res);
}
//------------------------------------------------------------------------------
void loop()
{
  // Really only needed write once in contiguous mode. Could move to setup.         
  mcp342xWrite(adcConfig); 
  int32_t data;
  if (!mcp342xRead(data)) halt();
  // voltage in millivolts
  double mv = (double)data/mvDivisor;
  // convert reading to microvolts
  int32_t uv = (data*1000L)/mvDivisor;
  Serial.print(mv);Serial.print(" mv, ");
  Serial.print(uv);Serial.println(" uv");
  delay(1000);
}
