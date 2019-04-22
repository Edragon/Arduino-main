char received[13];
int start_flag = 0;
int end_flag = 0;


void setup()
{
  Serial.begin(9600);
  Serial.println("Start, serial read and print in HEX");
}

void loop()
{

  if (Serial.available() > 0)

      Serial.print(Serial.read());


}
