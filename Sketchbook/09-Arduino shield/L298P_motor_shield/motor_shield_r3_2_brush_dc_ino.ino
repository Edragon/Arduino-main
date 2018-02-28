
int MotorPinDirection = 12;
int MotorPinSpeed = 3;

int ServoPinDirection = 13;
int ServoPinSpeed = 11;

int Remote_F = 7;
int Remote_B = 6;
int Remote_L = 5;
int Remote_R = 4;

void setup() {
  pinMode (MotorPinDirection, OUTPUT);
  pinMode (MotorPinSpeed, OUTPUT);
  pinMode (ServoPinDirection, OUTPUT);
  pinMode (ServoPinSpeed, OUTPUT);
  pinMode (Remote_F, INPUT);
  pinMode (Remote_B, INPUT);
  pinMode (Remote_L, INPUT);
  pinMode (Remote_R, INPUT);
}

void loop(){
  
  if( digitalRead(Remote_F)==HIGH )
  {
     digitalWrite(MotorPinDirection,255);
     analogWrite(ServoPinDirection,255);
     digitalWrite(ServoPinSpeed, HIGH);
     digitalWrite(MotorPinSpeed, HIGH);     
  }
    if(digitalRead(Remote_B)==HIGH)
    {
     digitalWrite(MotorPinDirection,0);
     analogWrite(ServoPinDirection,0);
     digitalWrite(ServoPinSpeed, HIGH);
     digitalWrite(MotorPinSpeed, HIGH);        
    }
    if(digitalRead(Remote_L)==HIGH)
      {
      digitalWrite(MotorPinDirection,255);
     analogWrite(ServoPinDirection,0);
     digitalWrite(ServoPinSpeed, HIGH);
     digitalWrite(MotorPinSpeed, HIGH);        
      }
     if(digitalRead(Remote_R)==HIGH)
     {
      digitalWrite(MotorPinDirection,0);
     analogWrite(ServoPinDirection,255);
     digitalWrite(ServoPinSpeed, HIGH);
     digitalWrite(MotorPinSpeed, HIGH);        
      }
    if(digitalRead(Remote_F)==LOW && digitalRead(Remote_B)==LOW && digitalRead(Remote_L)==LOW && digitalRead(Remote_R)==LOW)
    {
    digitalWrite(MotorPinSpeed, LOW);
    digitalWrite(ServoPinSpeed, LOW);
    }
}

