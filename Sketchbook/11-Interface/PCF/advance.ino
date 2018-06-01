void PCF_8574_read_write() {
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
    PCF_01.write(5, 1);
    delay(500);
    PCF_01.write(5, 0);

  }
  if (x == pres)  {
    Serial.println("detect - pres");
    PCF_01.write(5, 1);
    delay(2000);
    PCF_01.write(5, 0);

  }
  if (x == down)  {
    Serial.println("detect - down");
    PCF_01.write(5, 1);
    delay(500);
    PCF_01.write(5, 0);
    delay(500);
    PCF_01.write(5, 1);
    delay(500);
    PCF_01.write(5, 0);

  }
  if (x == right) {
    Serial.println("detect - right");
    PCF_01.write(5, 1);
    delay(100);
    PCF_01.write(5, 0);
    delay(100);
    PCF_01.write(5, 1);
    delay(100);
    PCF_01.write(5, 0);
    PCF_01.write(5, 1);
    delay(100);
    PCF_01.write(5, 0);
    delay(100);
    PCF_01.write(5, 1);
    delay(100);
    PCF_01.write(5, 0);
  }
}
