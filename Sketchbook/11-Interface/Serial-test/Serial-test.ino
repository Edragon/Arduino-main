int i = 0;
char received[13];


// connect EL125 ground to arduino ground
// connect EL125 TX pin to arduino D0 pin
//

void setup()
{
  Serial.begin(9600);
  Serial.println("Start, serial read and print in HEX");
}

void loop()
{

  if (Serial.available() > 0)
  {
    received[i] = Serial.read();
    Serial.print(received[i], HEX);
    //Serial.print(received[i], DEC);

    Serial.print("\t");
    i++;
  }
  //received[i] = '\0';

  if (i >= 13) {
    Serial.println("");
    
    Serial.print("card number hex is:");
    for (int j = 1; j < 11; j++ )
    {
      Serial.print(received[j], HEX);
    }
    Serial.println("");
    i = 0;
  }



}
