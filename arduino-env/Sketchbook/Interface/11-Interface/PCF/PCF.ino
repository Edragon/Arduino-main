//I2C device found at address 0x20  !   //8574
//I2C device found at address 0x48  !

#include "PCF8574.h"
#include <Wire.h>

byte top =   0b11111110;
byte left =  0b11111101;
byte pres =  0b11111011;
byte down = 0b11110111;
byte right =  0b11101111;

// adjust addresses if needed
PCF8574 PCF_01(0x20); 

#define PCF8591 (0x48)

byte adcvalue0, adcvalue1, adcvalue2, adcvalue3;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  

  PCF_8574_read_joystick_write_buzzer(); // this one not working !! please use another sketch!
  
  //PCF_8591_write(); // DAC, dim LED

  // sensor AIN3, pot trim on AIN2
  //PCF_8591_read();  //ADC

  //PCF_8574_read_joystick();

  //PCF_8574_write_buzzer(1000);  // fucntion beep buzzer every one second
}


