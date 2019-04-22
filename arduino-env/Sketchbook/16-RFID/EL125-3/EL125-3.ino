
// only connect EL125 TX pin to arduino RX D0 pin

void setup()
{
  Serial.begin(9600);
  Serial.println("Start, serial read and print in HEX");
}

void loop()
{
  if (Serial.available()) {
      Serial.write(Serial.read());
  }
  
  
}
