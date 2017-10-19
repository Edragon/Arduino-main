String comdata = "";
void setup()
{
  Serial.begin(9600);
  for(int i = 2; i <= 13; i++) pinMode(i, OUTPUT);
}

void loop()
{
  while (Serial.available() > 0)
  {
    comdata += int(Serial.read()) - '0';
    delay(2);
  }
  if(comdata.length() > 0)
  {
    for(int i = 0; i < comdata.length(); i++)
    {
      if(comdata[i]=='0'||comdata[i]=='1')
      {
        digitalWrite(i + 2, comdata[i] - '0');
        Serial.print("Pin ");
        Serial.print(i + 2);
        Serial.print(" is ");
        Serial.println(comdata[i]);
      }
    }
    comdata = "";
  }
}
