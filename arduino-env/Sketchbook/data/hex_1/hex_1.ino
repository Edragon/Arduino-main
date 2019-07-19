

void setup()
{
  Serial.begin (9600) ;
  byte data[3] ;
  data[0] = 0x81 ;
  data[1] = 0x82 ;
  data[2] = 0x83 ;
  unsigned long n = data[2] | (data[1] << 8) | (data[0] << 16);
  Serial.println (n, HEX) ;
  
  unsigned long res = data[0] ; res <<= 8 ;  // all the shifting is in unsigned long context
  res |= data[1] ; res <<= 8 ;
  res |= data[2] ;
  Serial.println (res, HEX) ;
}

void loop()
{
}
