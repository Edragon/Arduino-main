#include <AccelStepper.h>


// Define a stepper motor 1 for arduino 
// direction Digital 9 (CW), pulses Digital 8 (CLK)

AccelStepper stepper(1, 8, 9);

void setup()
{  
  // Change these to suit your stepper if you want
  
  stepper.setMaxSpeed(1000);//1100
  stepper.setAcceleration(1100);
  stepper.moveTo(2000);
}

void loop()
{
    // If at the end of travel go to the other end
    if (stepper.distanceToGo() == 0){
      stepper.moveTo( -stepper.currentPosition() );
    }
    
    stepper.run();
}