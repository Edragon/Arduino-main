int i = 0;
char received[13];
int flag_print_card_num = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Start, serial read and print in HEX");
}

void loop()
{

  if (Serial.available() > 0)
  {
    flag_print_card_num = 1;
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
