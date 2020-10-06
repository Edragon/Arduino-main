byte freqpin = 2;
unsigned long timeone;
unsigned int frequency;
unsigned int timediff;
unsigned int tcount = 0;
// sample time of 1 second or 1000 ms.
unsigned int sampletime = 1000;
unsigned long sampletimetwo = 1000000;
unsigned long j;

volatile unsigned int count = 0;

void setup()
{

  pinMode(freqpin, INPUT);
  // set interrupt pin 2 and use rising ( from gnd to +5 )
  attachInterrupt(0, freqinput, RISING);
  Serial.begin(9600);
}

void loop()
{
  /* Use while() loop and use millis().
      Calculate the time difference and compare the
      value with the sample time.
  */
  // activate the interrupt
  tcount = 0;
  count = 0;
  timediff = 0;
  attachInterrupt(0, freqinput, RISING);
  //interrupts();
  timeone = millis();
  while (timediff < sampletime)
  {
    tcount = count;
    timediff = millis() - timeone;
  }
  // de-activated the interrupt
  detachInterrupt(0);
  //noInterrupts();
  // Calculated frequency
  frequency = tcount;

  // Display frequency
  Serial.print("The difference of the delay is : ");
  Serial.println(timediff, DEC);
  Serial.print("Test while - ");
  Serial.print("Frequency : ");
  Serial.println(frequency, DEC);
  delay(3000);

  count = 0;
  tcount = 0;

  /* Using a for() loop design. no need for millis() but
      you need to fine tune the constant sampletime to set
     the proper time for sampling.
  */
  // activate the interrupts

  // interrupts();
  attachInterrupt(0, freqinput, RISING);
  timeone = millis();
  for (j = 0; j < sampletimetwo; j++)
  {
    tcount = count;
  }
  timediff = millis() - timeone;
  // de-activated the interrupt
  //noInterrupts();
  detachInterrupt(0);
  // Calculated frequency
  frequency = tcount;

  // Display frequency
  Serial.print("The time difference is : ");
  Serial.println(timediff, DEC);
  Serial.print("Test Loop - ");
  Serial.print("Frequency : ");
  Serial.println(frequency, DEC);
  Serial.println("\n");
  delay(2000);

}

// The interrupt routine
void freqinput ()
{
  count++;
}
