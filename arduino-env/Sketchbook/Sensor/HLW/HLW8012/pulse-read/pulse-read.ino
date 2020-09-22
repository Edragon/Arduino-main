
/*
  size = 3276
  Version 1.0 B

  To measure a frequency pulse and display in the serial monitor.

  Connect the pulse source to pin 2.
  The source have to be a +5 V to Gnd signal.

  In this experiment, I use a second Arduino to generated
  a pulse.  A 555 in astable Mode can be use.
  Just connect pin 3 of the 555 to pin 2 of the Arduino.

  Program by Serge J Desjardins aka techone
  Toronto, Ontario, Canada

  Compile and Tested.

*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

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
  lcd.init();                      // initialize the lcd
  lcd.backlight();
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
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Frequency : ");
  lcd.setCursor(0, 1);
  lcd.print(frequency, DEC);
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


