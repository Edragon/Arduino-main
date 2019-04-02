void PCF_8574_read_joystick_write_buzzer() {
  PCF_01.begin();
  int x = PCF_01.read8();
  Serial.print("Read ");
  Serial.println(x, BIN);
  //Serial.println(x, HEX);
  delay(100);
  if (x == top)   {
    Serial.println("detect - top");

  }
  if (x == left)  { // short beep
    Serial.println("detect - left");
    PCF_01.write(5, 0);
    delay(200);
    PCF_01.write(5, 1);

  }
  if (x == pres)  {
    Serial.println("detect - pres");
    PCF_01.write(5, 0);
    delay(1000);
    PCF_01.write(5, 1);

  }
  if (x == down)  {
    Serial.println("detect - down");
    PCF_01.write(5, 0);
    delay(200);
    PCF_01.write(5, 1);
    delay(200);
    PCF_01.write(5, 0);
    delay(200);
    PCF_01.write(5, 1);

  }
  if (x == right) {
    Serial.println("detect - right");
    PCF_01.write(5, 0);
    delay(50);
    PCF_01.write(5, 1);
    delay(50);
    PCF_01.write(5, 0);
    delay(50);
    PCF_01.write(5, 1);
    delay(50);
    PCF_01.write(5, 0);
    delay(50);
    PCF_01.write(5, 1);
    delay(50);
    PCF_01.write(5, 0);
    delay(50);
    PCF_01.write(5, 1);
  }
}
