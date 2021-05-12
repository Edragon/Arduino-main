// TB6600 pulse smallest = 0.00005 = 50 us


int pulse = 9;        
int DIR = 8;          

int pulse_delay = 80;
int addit = 5;


// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(pulse, OUTPUT);
  pinMode(DIR, OUTPUT);
  digitalWrite(DIR, HIGH);

}


void loop(){
  
    digitalWrite(pulse, HIGH);
    
//    pulse_delay = pulse_delay + addit;
//    if ( pulse_delay > 1000) {
//      pulse_delay =0;
//      }
    //delay(1);
    delayMicroseconds(500);
    //delayMicroseconds(pulse_delay);
    
    digitalWrite(pulse, LOW);

    
}



