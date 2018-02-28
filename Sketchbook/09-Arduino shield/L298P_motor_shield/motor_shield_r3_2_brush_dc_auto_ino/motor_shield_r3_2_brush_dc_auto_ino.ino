
int MotorPinDirection = 12;
int MotorPinSpeed = 3;

int ServoPinDirection = 13;
int ServoPinSpeed = 11;


void setup() {
  pinMode (MotorPinDirection, OUTPUT);
  pinMode (MotorPinSpeed, OUTPUT);
  pinMode (ServoPinDirection, OUTPUT);
  pinMode (ServoPinSpeed, OUTPUT);
}

void loop(){
     analogWrite(ServoPinSpeed, 0);
     digitalWrite(MotorPinDirection,255); 
     analogWrite(MotorPinSpeed, 150);     
     delay(1000);       
     analogWrite(MotorPinSpeed, 255);         
     delay(1000); 
      analogWrite(MotorPinSpeed, 0);
     digitalWrite(ServoPinDirection,255);
     analogWrite(ServoPinSpeed, 150);     
     delay(1000);       
     analogWrite(ServoPinSpeed, 255);     
     delay(1000);  
     digitalWrite(MotorPinDirection,255);
     digitalWrite(ServoPinDirection,255);
     analogWrite(MotorPinSpeed, 150);           
     analogWrite(ServoPinSpeed, 150);     
     delay(1000); 
      digitalWrite(MotorPinDirection,0);
     digitalWrite(ServoPinDirection,0);
     analogWrite(MotorPinSpeed, 150);            
     analogWrite(ServoPinSpeed, 150);     
     delay(1000);    
      digitalWrite(MotorPinDirection,255);
     digitalWrite(ServoPinDirection,0);
     analogWrite(MotorPinSpeed, 100);            
     analogWrite(ServoPinSpeed, 100);     
     delay(1000);
      digitalWrite(MotorPinDirection,0);
     digitalWrite(ServoPinDirection,255);
     analogWrite(MotorPinSpeed, 100);            
     analogWrite(ServoPinSpeed, 100);     
     delay(1000);     
}

