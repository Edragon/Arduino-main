//Pin connected to ST_CP of 74HC595
int latchPin = 13;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;


int LED_SEG_TAB[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0x01,0xee,0x3e,0x1a,0x7a,0x9e,0x8e,0x01};
                  //0     1    2     3    4    5    6    7    8    9    0    .    a    b    c    d    e    f.


void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  // count from 0 to 255 and display the number 
  // on the LEDs
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[0]);  
    shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[1]);  
    shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[2]);  
    shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[3]+1);  
    digitalWrite(latchPin, HIGH);

    delay(1000);
    
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[14]);  
    shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[13]);  
    shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[12]);  
    shiftOut(dataPin, clockPin, LSBFIRST, LED_SEG_TAB[11]);  
    digitalWrite(latchPin, HIGH);

    delay(1000);    
}

