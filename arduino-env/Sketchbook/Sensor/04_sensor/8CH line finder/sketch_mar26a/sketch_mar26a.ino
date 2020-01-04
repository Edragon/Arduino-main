*/

#include <SoftPWMServo.h>

//Setting up the Hardware pins
// First the line following (IR) sensors
const int irLeft = 2; //Left line sensor is on pin A2
const int irRight = 3; //Right line sensor is on pin A3

//Setting up the Arduino Motor Shield
const int leftDIR = 12; 
const int rightDIR = 13;
const int leftPWM = 3;
const int rightPWM = 11;
const int leftBrake = 9;
const int rightBrake = 8;
const char bothSpeed = 100; //sets how fast the motors will spin (0 to 255)

//Here we set up variable that will hold the ADC value representing the line sensor values
int leftSees = 0; //A2 ADC value (0 to 1023)
int rightSees = 0; //A3 ADC value (0 to 1023)

void setup() 
{ 

  //Make sure to set all of our control signal pins as output
  pinMode(leftDIR, OUTPUT); 
  pinMode(rightDIR, OUTPUT); 
  pinMode(leftBrake, OUTPUT); 
  pinMode(rightBrake, OUTPUT); 

  //Next we make sure our brake signals are set LOW
  digitalWrite(leftBrake, LOW);
  digitalWrite(rightBrake, LOW);
} 

void loop() 
{ 
    //Start by reading the left sensor on A2
  int leftEye = analogRead(irLeft);

  //delay a little bit
  delay(5);

  //next read the right sensor connected A3
  int rightEye = analogRead(irRight);

  //Next, we run the motors based on the sensor reading

  //If both sensors see black (ADC value greater than 1000), then back up
  if ((leftEye >= 1000)&&(rightEye >= 1000)) reverse();

  //Otherwise, if only the left sensor sees black, then turn off the left motor
  //so the robot veer to the left
  else if ((leftEye >= 1000)&&(rightEye < 1000)) turnLeft();

  //Otherwise, if only the right sensor sees black, then turn off the right motor
  //so the robot veer to the right
  else if ((leftEye < 1000)&&(rightEye >= 1000)) turnRight();

  //Otherwise, move forward
  else forward();

}

//Turn right by turning off the right motor 
//i.e disable the PWM to that wheel
void turnRight(void)
{
  digitalWrite(leftDIR, HIGH); 
  digitalWrite(rightDIR, HIGH);
  SoftPWMServoPWMWrite(leftPWM, bothSpeed);
  SoftPWMServoPWMWrite(rightPWM, 0);
}

//Turn left by turning off the left motor
//i.e disable the PWM to that wheel
void turnLeft(void)
{
  digitalWrite(leftDIR, HIGH); 
  digitalWrite(rightDIR, HIGH);
  SoftPWMServoPWMWrite(leftPWM, 0);
  SoftPWMServoPWMWrite(rightPWM, bothSpeed);
}

//Move forward by enabling both wheels
void forward(void)
{
  digitalWrite(leftDIR, HIGH); 
  digitalWrite(rightDIR, HIGH);
  SoftPWMServoPWMWrite(leftPWM, bothSpeed);
  SoftPWMServoPWMWrite(rightPWM, bothSpeed);
}

//Reverse by enabling both wheels 
void reverse(void)
{
  digitalWrite(leftDIR, LOW); 
  digitalWrite(rightDIR, LOW);
  SoftPWMServoPWMWrite(leftPWM, bothSpeed);
  SoftPWMServoPWMWrite(rightPWM, bothSpeed);
}
