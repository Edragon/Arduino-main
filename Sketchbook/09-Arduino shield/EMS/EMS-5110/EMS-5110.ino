#include <SPI.h>
#include <LCD5110_Graph_SPI.h>
#include "EmonLib.h"   

#define WINDOW 15
#define DELTA 1500

EnergyMonitor emon1; 

double sIrms;
float Pcur;
float Pmax;
float Pmin=100; 

LCD5110 myGLCD(5,6,3);   // need to re-define

extern unsigned char SmallFont[];

unsigned long measureTime;
boolean flag=false;
double delta=0;

#define DELTAMEASURE 30000

void setup(void)
{
  myGLCD.InitLCD(70);
  myGLCD.setFont(SmallFont);

  myGLCD.update();
  
  emon1.current(0, 111.1);             // Current: input pin, calibration.
  double Irms[WINDOW];
  // Calibrate (find offset)
  double cIrms = 0;
  flag = false;
  
  while (!flag) {
    myGLCD.print("calibrating", 0, 0);
    myGLCD.update();
    Irms[0] = emon1.calcIrms(1480); // the first value in the measurement obviously "crooked"
      //Serial.println("calculate delta");
      for (int i=0; i<WINDOW; i++) {
        Irms[i] = emon1.calcIrms(1480);
        cIrms = cIrms + Irms[i];
        delay(100);
      }
      delta = cIrms/WINDOW;
      flag = true;
  }

//myGLCD.print(" ready", 0, 0);
    //myGLCD.update();    

}

void loop(void)
{
  // write the current value
  sIrms = emon1.calcIrms(1480) - delta;  // Calculate Irms only
  sIrms = abs(sIrms);
  Pcur = sIrms*0.220;
  
    // the received data is printed
    char tbuf[6];
    dtostrf(Pcur,5,5,tbuf);
    myGLCD.print(tbuf, 0, 0);
    dtostrf(analogRead(A3),4,0,tbuf);
    myGLCD.print(tbuf, 30, 15);
  
    myGLCD.update();
    
    delay(250);
  
}
