////////////////////////////////////////////////////////////////////////
// by John Christian
//    potus98@yahoo.com
//    potus98.com
//    @potus98 on twitter
//
// Code below is not entirely original. It may have snippets or entire
// sections from other examples or tutorials found online. Thank you
// wonderful Internet for the help. If you see something you feel should
// be specifically credited, please let me know! 
//
// Pololu is awesome. They have great customer service, great prices,
// and great prodoucts. Not affiliated in any way. Just a satisfied customer.
// In fact, code below is HEAVILY based on Pololu example code.
////////////////////////////////////////////////////////////////////////

// This example uses a Pololu QTR-8A IR Reflectance Sensor and an 
// Arduino Mega 2560

// The emitter control pin can optionally be connected to digital pin 2, or you can leave
// it disconnected and change the EMITTER_PIN #define below from 2 to QTR_NO_EMITTER_PIN.

// The setup phase of this example calibrates the sensor for ten seconds and turns on
// the pin 13 LED while calibration is going on.

// The main loop of the example reads the calibrated sensor values and uses them to
// estimate the position of a line.  You can test this by taping a piece of 3/4" black
// electrical tape to a piece of white paper and sliding the sensor across it.  It
// prints the sensor values to the serial monitor as numbers from 0 (maximum reflectance) 
// to 9 (minimum reflectance) followed by the estimated location of the line as a number
// from 0 to 7000.  1000 means the line is directly under sensor 1, 2000 means directly
// under sensor 2, etc.  0 means the line is directly under sensor 0 or was last seen by
// sensor 0 before being lost.  7000 means the line is directly under sensor 7 or was
// last seen by sensor 7 before being lost.

// Use a global #define statement to enable/disable all print statements at compile time
// useful for turning off all Serial.print or Serial.println statements when I know the
// bot won't be connected to a USB serial port.

//#define debugprint Serial.print
//#define debugprintln Serial.println
// Or to disable
#define debugprint //
#define debugprintln //

////////////////////////////////////////////////////////////////////////
// Prepare QTR-8A IR Reflectance Array
#include <QTRSensors.h>
#define NUM_SENSORS             8  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define EMITTER_PIN             27  // emitter is controlled by digital pin 2
// sensors 0 through 7 are connected to analog inputs 0 through 5, 7, and 8, respectively
//QTRSensorsAnalog qtra((unsigned char[]) {0, 1, 2, 3, 4, 5, 7, 8}, 
QTRSensorsAnalog qtra((unsigned char[]) {8, 9, 10, 11, 12, 13, 14, 15}, // new wiring harness
NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

// Include PID library
#include <PID_v1.h>

////////////////////////////////////////////////////////////////////////
//
// Now using Arduino Mega 2560 with Sparkfun Ardumoto Shield 
//
// NXT pins (not Arduino pins, 
//  1 white analog, 9V supply
//  2 black ground, 
//  3 red   ground
//  4 green ipowerA, supply with 5V to activate encoders (I think?)
//  5 yelow encoder, RS-485 B
//  6 blue  encoder, RS-485 A
//
// ardumoto plugged onto arduino mega 2560
// supplying 9V power supply to Arduino Mega itself via barrel plug AND
// supplying 9V power to ArduMoto shield via VIN +/- terminals
// ardumoto A1 connected to NXT pin 1 AN IN
// ardumoto A2 connected to NXT pin 2 GRND
// ardumoto B3 connected to NXT pin 2 GRND
// ardumoto B4 connected to NXT pin 1 AN IN
//
// This example does not utilize the wheel encoders built into the
// Lego NXT servos.

// Initialize variables related to Ardumoto controlled motors
int pwm_a = 3;  //PWM (Pulse Width Modulation) control for motor outputs 1 and 2 is on digital pin 3
int pwm_b = 11;  //PWM control for motor outputs 3 and 4 is on digital pin 11
int dir_a = 12;  //direction control for motor outputs 1 and 2 is on digital pin 12
int dir_b = 13;  //direction control for motor outputs 3 and 4 is on digital pin 13

// Initialize variables related to LED indicator and pushbutton
// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

// Initialize variables related to PID
double PIDsetpoint, PIDinput, PIDoutput;
PID myPID(&PIDinput, &PIDoutput, &PIDsetpoint,2,5,1, DIRECT);

////////////////////////////////////////////////////////////////////////
// Prepare 
////////////////////////////////////////////////////////////////////////
void setup()
{
  // initialize serial communication
  Serial.begin(9600);
  // SETUP drive motors
  pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);

  // initialize LED pin as an output
  pinMode(ledPin, OUTPUT);

  // initialize pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  // initialize PID variables we're linked to
  PIDinput = 3500;               // assume we're starting off centered over line
  PIDsetpoint = 3500;            // attempt to stay centered over line
  myPID.SetMode(AUTOMATIC);      // turn the PID on

  // calibrate IR array (might move this into a function later)
  delay(500);
  int i;

  for (i = 0; i < 250; i++)      // make the calibration take about 7 seconds
  {
    digitalWrite(ledPin, HIGH);  // turn on LED (flicker LED during calibration)
    delay(20);
    qtra.calibrate();            // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
    digitalWrite(ledPin, LOW);   // turn off LED
    delay(20);

  }
  digitalWrite(ledPin, LOW);     // turn off LED to indicate we are through with calibration
  // print the calibration MINimum values measured when emitters were on
  for (i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtra.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  // print the calibration MAXimum values measured when emitters were on
  for (i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtra.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);

  // pause at end of setup to wait for button push
  pause(); 

}

////////////////////////////////////////////////////////////////////////
void loop()
{

  debugprintln ("Starting void loop");
  //navigationLogicA();     // has 3 conditions: straight, turn left, turn right
  //navigationLogicB();     // has 7 states with varying motor responses
  //navigationLogicC();     // uses PID library
  navigationLogicD();     // derived from B, but handles 90 degree intersections and turns
  //navigationSensorTest();   // display reflectance sensor array reading

//end of void loop  
}

////////////////////////////////////////////////////////////////////////
/* FUNCTIONS  */
////////////////////////////////////////////////////////////////////////

int pause(){
  // standby until momentary button is pressed
  // useful after calibration is complete, but before the event starts
  allStop(); // probably not necessary but may use in future
  buttonState = digitalRead(buttonPin);
  // buttsonState is HIGH while untouched, pressed button causes LOW
    while(buttonState == HIGH){
    buttonState = digitalRead(buttonPin);
    digitalWrite(ledPin, HIGH);    // turn on LED (slow blink waiting)
    delay(250);
    digitalWrite(ledPin, LOW);     // turn off LED
    delay(250); 
  }
  Serial.println("Button pressed! Start 5 second delay...");
  delay(5000);
  Serial.println("Time's up! Leaving pause loop.");
}

int allStop(){
  // turn off drive motors
  analogWrite(pwm_a, 0);   // motor A off ("A" refers to Ardumoto output terminals A1 and A2)
  analogWrite(pwm_b, 0);   // motor B off ("B" refers to Ardumoto output terminals B3 and B4)
}

int driveForward(int speedMotorA, int speedMotorB){
  Serial.print("driveForward function: ");
  Serial.print(speedMotorA);
  Serial.print(" ");
  Serial.print(speedMotorB);
  // function accepts two arguments: speed for motorA, motorB
  // future args may include specific time to run
  // or distance to cover (if using wheel encoder)
  digitalWrite(dir_a, LOW);  //Set motor direction, 1 low, 2 high
  digitalWrite(dir_b, LOW);  //Set motor direction, 3 high, 4 low
  analogWrite(pwm_a, speedMotorA);
  analogWrite(pwm_b, speedMotorB);
}

/*
int driveForward(int speedMotorA, int speedMotorB){  // fake driveForward for testing
  Serial.print("driveForward function: ");
  Serial.print(speedMotorA);
  Serial.print(" ");
  Serial.print(speedMotorB);
  // function accepts two arguments: speed for motorA, motorB
  // future args may include specific time to run
  // or distance to cover (if using wheel encoder)
  //digitalWrite(dir_a, LOW);  //Set motor direction, 1 low, 2 high
  //digitalWrite(dir_b, LOW);  //Set motor direction, 3 high, 4 low
  //analogWrite(pwm_a, speedMotorA);
  //analogWrite(pwm_b, speedMotorB);
}
*/

int driveBackward(int speedMotorA, int speedMotorB){
  // function accepts two args: speed for motorA, motorB
  // future args may include specific time to run
  // or distance to cover (for use with countable wheels)
  digitalWrite(dir_a, HIGH);  //Set motor direction, 1 low, 2 high
  digitalWrite(dir_b, HIGH);  //Set motor direction, 3 high, 4 low
  analogWrite(pwm_a, speedMotorA);
  analogWrite(pwm_b, speedMotorB);
}

int pivotCounterClockwise(int speedMotor, int pivotTime){
  // function accepts two args: speed for motors and time to run
  // pivots counter clockwise
  // speedMotor adjusts how fast to pivot. >100 good to overcome standing resistance
  // pivotTime adjusts how long to pivot
  //
  // Based on tests with reasonably new batteries
  // 180 - 210 degrees = 110,1000
  //  40 -  80 degrees = 110, 400
  //  50 - 110 degrees = 100, 500
  // Bleh! Trying to use power and timing to perform semi-accurate turns doesn't seem feasible
  // with current setup. The resulting degrees of turn vary wildly. Will need to access the
  // wheel encoder information to make useful pivots.
  //
  int speedMotorA = speedMotor;
  int speedMotorB = speedMotor;
  Serial.print("pivotCounterClockwise function: ");
  Serial.print(speedMotorA);
  Serial.print(" ");
  Serial.print(speedMotorB);
  Serial.print(" Time: ");
  Serial.println(pivotTime);
  digitalWrite(dir_a, LOW);
  digitalWrite(dir_b, HIGH);
  analogWrite(pwm_a, speedMotorA);
  analogWrite(pwm_b, speedMotorB);
  delay(pivotTime);
}

int pivotClockwise(int speedMotor, int pivotTime){
  // function accepts two args: speed for motors and time to run
  // pivots counter clockwise
  // speedMotor adjusts how fast to pivot. >100 good to overcome standing resistance
  // pivotTime adjusts how long to pivot
  //
  // Based on tests with reasonably new batteries
  // 180 - 210 degrees = 110,1000
  //  40 -  80 degrees = 110, 400
  //  50 - 110 degrees = 100, 500
  // Bleh! Trying to use power and timing to perform semi-accurate turns doesn't seem feasible
  // with current setup. The resulting degrees of turn vary wildly. Will need to access the
  // wheel encoder information to make useful pivots.
  //
  int speedMotorA = speedMotor;
  int speedMotorB = speedMotor;
  Serial.print("pivotClockwise function: ");
  Serial.print(speedMotorA);
  Serial.print(" ");
  Serial.print(speedMotorB);
  Serial.print(" Time: ");
  Serial.println(pivotTime);
  digitalWrite(dir_a, HIGH);
  digitalWrite(dir_b, LOW);
  analogWrite(pwm_a, speedMotorA);
  analogWrite(pwm_b, speedMotorB);
  delay(pivotTime);
}

int setDirection(int positionToCheck){
  // function accepts the position value which should be between 1 and 6999 then
  // evaluate the position value and adjust motorA/B speeds to stay on course.
  // 
}

int BackupTurnFunction(){
  // This is a basic, pre-defined backup and turn routine. You could call this
  // when, say, a forward facing ping sensor detects an object in the bot's path
  digitalWrite(dir_a, HIGH);  // Set motor direction, 1 low, 2 high
  digitalWrite(dir_b, HIGH);  // Set motor direction, 3 high, 4 low
  analogWrite(pwm_a, 150);    //
  analogWrite(pwm_b, 75);     // running motors at different speed results in backup turn
  delay (3000);               // give the motor controller and motors a moment to come to a stop
  analogWrite(pwm_a, 0);      //
  analogWrite(pwm_b, 0);      //
  delay (2000);               // set both motors to off for a moment
  return 0;                   // may use return values in the future, but for now,
                              // this value is not used
}

////////////////////////////////////////////////////////////////////////
int navigationSensorTest(){
////////////////////////////////////////////////////////////////////////
  // Removed for print ability since this function is not in use
}

////////////////////////////////////////////////////////////////////////
int navigationLogicA(){
////////////////////////////////////////////////////////////////////////
  // Removed for print ability since this function is not in use
}

////////////////////////////////////////////////////////////////////////
int navigationLogicB(){
////////////////////////////////////////////////////////////////////////
  // Sort-of a poor man's PID
  // Removed for print ability since this function is not in use
}

////////////////////////////////////////////////////////////////////////
int navigationLogicC(){
////////////////////////////////////////////////////////////////////////
  // Actual PID
  // Removed for print ability since this function is not in use  
}

////////////////////////////////////////////////////////////////////////
int navigationLogicD(){
////////////////////////////////////////////////////////////////////////
Serial.println("Enterting navigationLogicD function");
  // Sort-of a poor man's PID
  //
  // Derived from navigationLogicB but adding 90 degree intersection and 90 degree turn handling
  //
  // read calibrated sensor values and obtain a measure of the line position from 0 to 7000.
  unsigned int position = qtra.readLine(sensorValues);
  unsigned char i;
  for (i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i] * 10 / 1001);
    Serial.print(' ');
  }
  Serial.print("  ");
  Serial.print(position);
  Serial.println("  ");
  ////////////////////////////////////////////////////////////////////////
  unsigned int sensors[8];
  // get calibrated sensor values returned in the sensors array, along with the line position
  // position will range from 0 to 7000, with 3000 corresponding to the line over sensor 4
  // Will be treating sensor 4 (array position 5) as middle sensor
  //int position = qtr.readLine(sensors);
  // if all eight sensors see very low reflectance (black), take some appropriate action for this situation
  // might need to revisit hardcoding 750. should consider deriving from min/max during calibration sequence

  // CHECK for all black (all sensors very low reflectance) won't need for line following loop course.
  if (sensorValues[0] > 750 && sensorValues[1] > 750 && sensorValues[2] > 750 && sensorValues[3] > 750 && sensorValues[4] > 750 && sensorValues[5] > 750 && sensorValues[6] > 750 && sensorValues[7] > 750)
  {
    Serial.println("    DETECTED: All Black");
  }
  if (sensorValues[0] < 80 && sensorValues[1] < 80 && sensorValues[2] < 80 && sensorValues[3] < 80 && sensorValues[4] < 80 && sensorValues[5] < 80 && sensorValues[6] < 80 && sensorValues[7] < 80)
  {
    Serial.println("    DETECTED: All White");
  }

  /*
  // CHECK for a 90 degree intersection (all sensors low reflectance) a more relaxed version of all black check
  if (sensorValues[0] > 400 && sensorValues[1] > 400 && sensorValues[2] > 400 && sensorValues[3] > 400 && sensorValues[4] > 400 && sensorValues[5] > 400 && sensorValues[6] > 400 && sensorValues[7] > 400)
  {
    Serial.print("    DETECTED: 90 degree intersection");
    // keep going straight
    driveForward(100,100);
    delay(100); // drive forward 1/10 second

  }

  // CHECK for a 90 degree LEFT
  if (sensorValues[0] > 400 && sensorValues[1] > 400 && sensorValues[2] > 400 && sensorValues[3] > 400)
  {
    Serial.print("    DETECTED: 90 degree Left");
    // perform 90 degree left turn  
  }

  // CHECK for a 90 degree RIGHT
  if (sensorValues[4] > 400 && sensorValues[5] > 400 && sensorValues[6] > 400 && sensorValues[7] > 400)
  {
    Serial.print("    DETECTED: 90 degree Right");
  }
  */

  // By this point, we're assuming no 90 degree intersection or 90 degree turns
  //
  // Determine drive action based on line position value between 0 and 7000.
  // This logic assumes a single line. No provision for intersections at this time.
  if (position == 0){
    Serial.println("    Left Pivot");
    //pivotCounterClockwise(80,0);  // Seems to pivot too fast.
    driveForward(85, 0);            // Try pivot on left wheel instead of center of vehicle.
  }
  else if (position > 0 && position < 1000){
    Serial.println("    Left Strong ");
    driveForward(90, 55);
  }
  else if (position >= 1000 && position < 2000){
    Serial.println("    Left Moderate ");
    driveForward(90, 75);
  }
  else if (position >= 2000 && position < 3000){
    Serial.println("    Left Slight ");
    driveForward(100, 85);
  }
  else if (position >= 3000 && position < 4000){
    Serial.println("    Straight Ahead ");
    driveForward(100, 100);
  }
  else if (position >= 4000 && position < 5000){
    Serial.println("    Right Slight ");
    driveForward(85, 100);
  }
  else if (position >= 5000 && position < 6000){
    Serial.println("    Right Moderate ");
    driveForward(75, 90);
  }
  else if (position >= 6000 && position < 7000){
    Serial.println("    Right Strong ");
    driveForward(55, 90);
  }
  else if (position >= 7000){
    Serial.println("    Right Pivot ");
    //pivotClockwise(80,0);        // Seems to pivot too fast.
    driveForward(0,85);            // Try pivot on left wheel instead of center of vehicle.
  }
Serial.println("end of navigationLogicD function");
//delay(500); // Remove this delay for final testing and actual competition
}
