int redpin = 12;   
int bluepin = 13;     
int yellowpin = 15;
int PROG_LED = 16;
int val;

void setup() {
  pinMode(redpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(yellowpin, OUTPUT);
  pinMode(PROG_LED, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  for (val = 255; val > 0; val--)
  {
    analogWrite(redpin, val);
    analogWrite(bluepin, 255 - val);
    analogWrite(yellowpin, 128 - val);
    delay(10);
  }
    digitalWrite(PROG_LED, HIGH);
  
  for (val = 0; val < 255; val++)
  {
    analogWrite(redpin, val);
    analogWrite(bluepin, 255 - val);
    analogWrite(yellowpin, 128 - val);
    delay(10);
  }
    digitalWrite(PROG_LED, LOW);
    
  Serial.println(val, DEC);
}
