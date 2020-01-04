
#include <Stepper.h>
 
 const int stepsPerRevolution = 48;  // change this to fit the number of steps per revolution
                          // for your motor
 
 // initialize the stepper library on the motor shield
 Stepper myStepper(stepsPerRevolution, 12,13);     
 
 // give the motor control pins names:
 const int pwmA = 3;
 const int pwmB = 11;
 const int brakeA = 9;
 const int brakeB = 8;
 const int dirA = 12;
 const int dirB = 13;
 
 int x = 0;
 
int analogPina = 0;     // analog pin 0
int vala = 0;           // variable to store the value read
int analogPinb = 1;     // analog pin 0
int valb = 0;           // variable to store the value read

void setup() {
 Serial.begin(9600);
 // set the PWM and brake pins so that the direction pins  // can be used to control the motor:
 pinMode(pwmA, OUTPUT);
 pinMode(pwmB, OUTPUT);
 pinMode(brakeA, OUTPUT);
 pinMode(brakeB, OUTPUT);
 digitalWrite(pwmA, HIGH);
 digitalWrite(pwmB, HIGH);
 digitalWrite(brakeA, LOW);
 digitalWrite(brakeB, LOW);
 // initialize the serial port:
 Serial.begin(9600);
 // set the motor speed (for multiple steps only):
 }

 void Current()
 {  
  vala = analogRead(analogPina);    // read the input pin
  valb = analogRead(analogPinb);    // read the input pin
   
  Serial.print("Channal A current : ");
  Serial.print(vala*4.88);             // debug value  
  Serial.print("mA");
  Serial.print("\t");
  Serial.print("Channal B current : ");
  Serial.print(valb*4.88);             // debug value  
  Serial.println("mA");
 }
 
 void loop() {
 
  myStepper.setSpeed(100);
  Current();
  myStepper.step(50);
  Current();
  myStepper.step(-50);
  Current();
  delay(200);  
  myStepper.setSpeed(500);
  myStepper.step(300);
  Current();
  myStepper.step(-300);
  delay(200);
  myStepper.setSpeed(1000);
  myStepper.step(500);
  Current();
  myStepper.step(-500);
  delay(200);
 }
