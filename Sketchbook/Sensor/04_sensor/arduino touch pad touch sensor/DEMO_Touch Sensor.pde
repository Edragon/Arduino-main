// www.electrodragon.com

int Button=2;		//connect button to D2
int LED=13;
void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(Button, INPUT);
  
}
 
void loop()
{
   
if(digitalRead(Button)==HIGH)	//when the digital output value of button is high, turn on the LED.
  {
    digitalWrite(LED, LOW);
   
  }
   
  if(digitalRead(Button)==LOW)	//when the digital output value of button is low, turn off the LED.
  {
    digitalWrite(LED, HIGH);
  }
 
}