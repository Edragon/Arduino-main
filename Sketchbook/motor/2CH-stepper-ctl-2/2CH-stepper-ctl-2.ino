 int Pin0 = 10; // or use pin 8
 int Pin1 = 11; // or use pin 9
 int Pin2 = 12; // or use pin 10
 int Pin3 = 13; // or use pin 11
 int _step = 0; 
 boolean dir = true;// change the direction of the motor
 void setup() 
 { 
  pinMode(Pin0, OUTPUT);  
  pinMode(Pin1, OUTPUT);  
  pinMode(Pin2, OUTPUT);  
  pinMode(Pin3, OUTPUT);  
 } 
  void loop() 
 { 
  switch(_step){ 
    case 0: 
      digitalWrite(Pin0, LOW);  
      digitalWrite(Pin1, LOW); 
      digitalWrite(Pin2, LOW); 
      digitalWrite(Pin3, HIGH); 
    break;  
    case 1: 
      digitalWrite(Pin0, LOW);  
      digitalWrite(Pin1, LOW); 
      digitalWrite(Pin2, HIGH); 
      digitalWrite(Pin3, HIGH); 
    break;  
    case 2: 
      digitalWrite(Pin0, LOW);  
      digitalWrite(Pin1, LOW); 
      digitalWrite(Pin2, HIGH); 
      digitalWrite(Pin3, LOW); 
    break;  
    case 3: 
      digitalWrite(Pin0, LOW);  
      digitalWrite(Pin1, HIGH); 
      digitalWrite(Pin2, HIGH); 
      digitalWrite(Pin3, LOW); 
    break;  
    case 4: 
      digitalWrite(Pin0, LOW);  
      digitalWrite(Pin1, HIGH); 
      digitalWrite(Pin2, LOW); 
      digitalWrite(Pin3, LOW); 
    break;  
    case 5: 
      digitalWrite(Pin0, HIGH);  
      digitalWrite(Pin1, HIGH); 
      digitalWrite(Pin2, LOW); 
      digitalWrite(Pin3, LOW); 
    break;  
      case 6: 
      digitalWrite(Pin0, HIGH);  
      digitalWrite(Pin1, LOW); 
      digitalWrite(Pin2, LOW); 
      digitalWrite(Pin3, LOW); 
    break;  
    case 7: 
      digitalWrite(Pin0, HIGH);  
      digitalWrite(Pin1, LOW); 
      digitalWrite(Pin2, LOW); 
      digitalWrite(Pin3, HIGH); 
    break;  
    default: 
      digitalWrite(Pin0, LOW);  
      digitalWrite(Pin1, LOW); 
      digitalWrite(Pin2, LOW); 
      digitalWrite(Pin3, LOW); 
    break;  
  } 
  if(dir){ 
    _step++; 
  }else{ 
    _step--; 
  } 
  if(_step>7){ 
    _step=0; 
  } 
  if(_step<0){ 
    _step=7; 
  } 
  delay(1);  // change this delay to control the speed of your stepper, delay to 100 you can see the signal LEDs blinking.
 }
