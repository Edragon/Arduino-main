#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX

int i = 0;
char received[3];


void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Start, serial read and print in HEX");
  
}

void loop()
{

  if (mySerial.available() > 0)
  {
    received[i] = mySerial.read();
    Serial.print(received[i], HEX);
    //Serial.print(received[i], DEC);

    Serial.print("\t");
    i++;
  }
  //received[i] = '\0';

  if (i >= 1) {
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
