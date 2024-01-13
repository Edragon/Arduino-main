
#define Signal_Pin 2 //Use Arduino Pin 2 or pin 3, both supports hardware interrupt
int counter = 0;


void setup()
{
  pinMode(Signal_Pin , INPUT_PULLUP);
  Serial.begin(9600);
  // 0 for D2, 1 for D3
  attachInterrupt(0, zero_crosss_int, RISING);  // Choose the zero cross interrupt # from the table above
}

void zero_crosss_int()  // function to be fired at the zero crossing to dim the light
{
  counter++;
}

void loop()
{
  Serial.println(counter);
}
