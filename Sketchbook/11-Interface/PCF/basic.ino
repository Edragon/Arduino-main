

void PCF_8591_write()
{
  for (int i = 0; i < 256; i++)
  {
    Wire.beginTransmission(PCF8591); // wake up PCF8591
    Wire.write(0x40); // control byte - turn on DAC (binary 1000000)
    Wire.write(i); // value to send to DAC
    Wire.endTransmission(); // end tranmission
    delay(5);
  }

  for (int i = 255; i >= 0; --i)
  {
    Wire.beginTransmission(PCF8591); // wake up PCF8591
    Wire.write(0x40); // control byte - turn on DAC (binary 1000000)
    Wire.write(i); // value to send to DAC
    Wire.endTransmission(); // end tranmission
    delay(5);
  }
}

void PCF_8591_read () {
  Wire.beginTransmission(PCF8591);
  Wire.write(0x04);
  Wire.endTransmission();
  Wire.requestFrom(PCF8591, 5);

  adcvalue0 = Wire.read(); // first byete is old data of last time read
  adcvalue0 = Wire.read();
  adcvalue1 = Wire.read();
  adcvalue2 = Wire.read();
  adcvalue3 = Wire.read();

  Serial.print("AIN0: ");
  Serial.print(adcvalue0);
  Serial.print(" ,");
  Serial.print("AIN1 Sensor: ");
  Serial.print(adcvalue1);  // sensor
  Serial.print(" ,");
  Serial.print("AIN2: ");
  Serial.print(adcvalue2);
  Serial.print(" ,");
  Serial.print("AIN3 Potentiometer: ");
  Serial.print(adcvalue3);  // Trim pot
  Serial.println();
  delay(1000);
}

void PCF_8574_read_joystick () {
  PCF_01.begin();
  int x = PCF_01.read8();
  Serial.print("Read ");
  Serial.println(x, BIN);
  //Serial.println(x, HEX);
  delay(500);
  if (x == top)   {
    Serial.println("detect - top");
  }
  if (x == left)  {
    Serial.println("detect - left");
  }
  if (x == pres)  {
    Serial.println("detect - pres");
  }
  if (x == down)  {
    Serial.println("detect - down");
  }
  if (x == right) {
    Serial.println("detect - right");
  }
}

void PCF_8574_off_buzzer() {
  PCF_01.begin();
  PCF_01.write(5, 0);
  delay(1000);
}

void PCF_8574_write_buzzer(int d)
{
  PCF_01.begin();
  //turn P0 (pin 4) of the I/O expander on and off once a second
  //PCF_01.write(6, 1);
  PCF_01.write(5, 1);
  //PCF_01.write(4, 1);
  //PCF_01.write(3, 1);
  //PCF_01.write(2, 1);
  //PCF_01.write(1, 1);
  //PCF_01.write(0, 1);
  delay(d);

  //PCF_01.write(6, 0);
  PCF_01.write(5, 0);
  //PCF_01.write(4, 0);
  //PCF_01.write(3, 0);
  //PCF_01.write(2, 0);
  //PCF_01.write(1, 0);
  //PCF_01.write(0, 0);
  delay(d);
}
