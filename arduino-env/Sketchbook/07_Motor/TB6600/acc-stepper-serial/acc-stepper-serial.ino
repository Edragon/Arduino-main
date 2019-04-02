// Control Stepper motors at the same time with different speeds
// and accelerations via serial commands

// Uses the the Adafruit Motor Shield V2 board
// http://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/overview

// Requires AccelStepper with AFMotor support (http://www.airspayce.com/mikem/arduino/AccelStepper/)
#include <AccelStepper.h>
// Requires the Adafruit Motor Shield V2 library (https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library)
#include <Wire.h>
#include <Adafruit_MotorShield.h>
//#include "utility/Adafruit_PWMServoDriver.h"

#define NUM_STEPPERS 12
#define STEP_MODE DOUBLE // you can change this to SINGLE, DOUBLE or INTERLEAVE or MICROSTEP

const char  eol = '\n'; // end of line character
const String stepperStart = " 1";
const String stepperStop = " 0";

boolean stepperRunning[NUM_STEPPERS] = {false, false, false, false, false, false,false, false, false, false, false, false};

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS0 = Adafruit_MotorShield(); // default board ID
Adafruit_MotorShield AFMS1 = Adafruit_MotorShield(0x61); // board ID + 1
Adafruit_MotorShield AFMS2 = Adafruit_MotorShield(0x62); // board ID + 2
Adafruit_MotorShield AFMS3 = Adafruit_MotorShield(0x63); // board ID + 3
Adafruit_MotorShield AFMS4 = Adafruit_MotorShield(0x64); // board ID + 4
Adafruit_MotorShield AFMS5 = Adafruit_MotorShield(0x65); // board ID + 5

// Connect a stepper motors with 200 steps per revolution (1.8 degree)
Adafruit_StepperMotor *motor0 = AFMS0.getStepper(200, 1);
Adafruit_StepperMotor *motor1 = AFMS0.getStepper(200, 2);
Adafruit_StepperMotor *motor2 = AFMS1.getStepper(200, 1);
Adafruit_StepperMotor *motor3 = AFMS1.getStepper(200, 2);
Adafruit_StepperMotor *motor4 = AFMS2.getStepper(200, 1);
Adafruit_StepperMotor *motor5 = AFMS2.getStepper(200, 2);
Adafruit_StepperMotor *motor6 = AFMS3.getStepper(200, 1);
Adafruit_StepperMotor *motor7 = AFMS3.getStepper(200, 2);
Adafruit_StepperMotor *motor8 = AFMS4.getStepper(200, 1);
Adafruit_StepperMotor *motor9 = AFMS4.getStepper(200, 2);
Adafruit_StepperMotor *motor10 = AFMS5.getStepper(200, 1);
Adafruit_StepperMotor *motor11 = AFMS5.getStepper(200, 2);

// wrappers for the Adafruit motors to work with AccelStepper
// first motor
void forwardstep0() { motor0->onestep(FORWARD, STEP_MODE); }
void backwardstep0() {  motor0->onestep(BACKWARD, STEP_MODE); }
// second motor
void forwardstep1() { motor1->onestep(FORWARD, STEP_MODE); }
void backwardstep1() { motor1->onestep(BACKWARD, STEP_MODE); }
// third motor
void forwardstep2() { motor2->onestep(FORWARD, STEP_MODE); }
void backwardstep2() { motor2->onestep(BACKWARD, STEP_MODE); }
// forth motor
void forwardstep3() { motor3->onestep(FORWARD, STEP_MODE); }
void backwardstep3() { motor3->onestep(BACKWARD, STEP_MODE); }
// fith motor
void forwardstep4() { motor4->onestep(FORWARD, STEP_MODE); }
void backwardstep4() { motor4->onestep(BACKWARD, STEP_MODE); }
// sixth motor
void forwardstep5() { motor5->onestep(FORWARD, STEP_MODE); }
void backwardstep5() { motor5->onestep(BACKWARD, STEP_MODE); }
// seventh motor
void forwardstep6() { motor6->onestep(FORWARD, STEP_MODE); }
void backwardstep6() {  motor6->onestep(BACKWARD, STEP_MODE); }
// eigth motor
void forwardstep7() { motor7->onestep(FORWARD, STEP_MODE); }
void backwardstep7() { motor7->onestep(BACKWARD, STEP_MODE); }
// ninth motor
void forwardstep8() { motor8->onestep(FORWARD, STEP_MODE); }
void backwardstep8() { motor8->onestep(BACKWARD, STEP_MODE); }
// tenth motor
void forwardstep9() { motor9->onestep(FORWARD, STEP_MODE); }
void backwardstep9() { motor9->onestep(BACKWARD, STEP_MODE); }
// eleventh motor
void forwardstep10() { motor10->onestep(FORWARD, STEP_MODE); }
void backwardstep10() { motor10->onestep(BACKWARD, STEP_MODE); }
// twelfth motor
void forwardstep11() { motor11->onestep(FORWARD, STEP_MODE); }
void backwardstep11() { motor11->onestep(BACKWARD, STEP_MODE); }

// wrap the motors in an AccelStepper object array
AccelStepper steppers[NUM_STEPPERS] = {
  AccelStepper(forwardstep0, backwardstep0),
  AccelStepper(forwardstep1, backwardstep1),
  AccelStepper(forwardstep2, backwardstep2),
  AccelStepper(forwardstep3, backwardstep3),
  AccelStepper(forwardstep2, backwardstep4),
  AccelStepper(forwardstep3, backwardstep5),
  AccelStepper(forwardstep0, backwardstep6),
  AccelStepper(forwardstep1, backwardstep7),
  AccelStepper(forwardstep2, backwardstep8),
  AccelStepper(forwardstep3, backwardstep9),
  AccelStepper(forwardstep2, backwardstep10),
  AccelStepper(forwardstep3, backwardstep11)
};

void setup() {  
    Serial.begin(57600);
    AFMS0.begin(); // start the Adafruit board
    AFMS1.begin(); // start the Adafruit board
    AFMS2.begin(); // start the Adafruit board
    AFMS3.begin(); // start the Adafruit board
    AFMS4.begin(); // start the Adafruit board
    AFMS5.begin(); // start the Adafruit board
}

void loop() {
  for( int i=0; i <  NUM_STEPPERS; i++) {
    steppers[i].run();
    // check to see if any moves have finished
    if (steppers[i].distanceToGo() == 0 && stepperRunning[i]) {
      String stepperIDstring = String(i) + stepperStop;
      Serial.println(stepperIDstring);
      stepperRunning[i] = false;
    }
  }
  
  if (Serial.available() > 0) {
    // get the parameters in the form: stepperID locationToMoveTo speed acceleration
    // e.g. 1 200 300 300 requests that the second motor moves to position 200 with a max speed of 300, and acceleration of 300
    int stepperID = Serial.parseInt();
    String stepperIDstring = String(stepperID);
    int location = Serial.parseInt();
    float speed = Serial.parseFloat();
    float acceleration = Serial.parseFloat();
    // read in the line end before we use the values
    if (Serial.read() == eol) {
      //speed = max(speed,1);
      acceleration = max(acceleration,1);
      if (stepperID >= 0 && stepperID < NUM_STEPPERS) {
        if (speed > 0) {
          steppers[stepperID].setMaxSpeed(speed);
          steppers[stepperID].setAcceleration(acceleration);
          steppers[stepperID].moveTo(location);
          stepperRunning[stepperID] = true;
          stepperIDstring += stepperStart;
          Serial.println(stepperIDstring);
        } else { // stop where we are when speed is less than 1
          steppers[stepperID].setAcceleration(acceleration);
          steppers[stepperID].stop();         
          steppers[stepperID].run();
          steppers[stepperID].run();
          stepperRunning[stepperID] = false;
          stepperIDstring += stepperStop;
          Serial.println(stepperIDstring);
        }
      }
    }
  }
}