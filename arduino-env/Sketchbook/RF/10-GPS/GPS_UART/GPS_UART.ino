void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // make a string for assembling the data to log:
  char index = 0;
  char temp = 0;
  String dataString = "";

   while(Serial.available())
   {
     temp = Serial.read();
     dataString += String(temp);
     index++;
     if(index>200)
       break;
   }
   Serial.print(dataString);
 }
