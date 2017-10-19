// Demonstration Arduino sketch for four digit, seven segment display with enclosure
// http://tronixlabs.com/display/led/numeric/four-digit-seven-segment-display-module-and-enclosure/

int latchPin = 12;   // connect to LCK pin
int clockPin = 13;   // connect to CLK pin
int dataPin = 11;    // connect to SDI pin

int LED_SEG_TAB[]={
  0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0x01,0xee,0x3e,0x1a,0x7a,0x9e,0x8e,0x01,0x00};
//0     1    2     3    4    5    6    7    8    9   dp   .    a    b    c    d    e    f   off

void setup() 
{
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void displayNumber(int value, boolean leadingZero)
// break down "value" into digits and store in a,b,c,d  
{
  int a,b,c,d;
  a = value / 1000;
  value = value % 1000;
  b = value / 100;
  value = value % 100;
  c = value / 10;
  value = value % 10;
  d = value;

  if (leadingZero==false) // removing leading zeros
  {
    if (a==0 && b>0)                  {
      a = 18;
    } 
    if (a==0 && b==0 && c>0)          {
      a = 18; 
      b = 18;
    }
    if (a==0 && b==0 && c==0)         {
      a = 18; 
      b = 18; 
      c = 18;
    }
    if (a==0 && b==0 && c==0 && d==0) {
      a = 18; 
      b = 18; 
      c = 18; 
      d = 18;
    }
  }

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[d]);  
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[c]);  
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[b]);  
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[a]);  
  digitalWrite(latchPin, HIGH);
}

void allOff() // turns off all segments
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, 0);  
  shiftOut(dataPin, clockPin, LSBFIRST, 0);  
  shiftOut(dataPin, clockPin, LSBFIRST, 0);  
  shiftOut(dataPin, clockPin, LSBFIRST, 0);  
  digitalWrite(latchPin, HIGH);
}

void loop() 
{
  for (int z=900; z<=1100; z++)
  {
    displayNumber(z, false);
    delay(10);
  }
  delay(1000);
  for (int z=120; z>=0; --z)
  {
    displayNumber(z, true);
    delay(10);
  }
  delay(1000);

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[14]);  
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[13]);  
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[12]);  
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[11]);  
  digitalWrite(latchPin, HIGH);

  delay(1000); 

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[16]);  
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[15]);  
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[14]);  
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[13]);  
  digitalWrite(latchPin, HIGH);
  delay(1000);

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[0]);  
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[1]);  
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[2]);  
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[3]+1);  
  digitalWrite(latchPin, HIGH);
  delay(1000);
  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[7]);  
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[6]+1);  
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[5]);  
  shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[4]);  
  digitalWrite(latchPin, HIGH);
  delay(1000);
}


