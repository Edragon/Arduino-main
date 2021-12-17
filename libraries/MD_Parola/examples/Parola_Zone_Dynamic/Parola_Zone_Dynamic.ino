// Program to demonstrate using dynamic (changing) zones with the MD_Parola library
//
// Zones are changed by 2 modules for each iteration and a simple string
// is displayed in the zone.

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#if USE_LIBRARY_SPI
#include <SPI.h>
#endif

// Turn on debug statements to the serial output
#define  DEBUG  0

#if  DEBUG
#define	PRINT(s, x)	{ Serial.print(F(s)); Serial.print(x); }
#define	PRINTS(x)	Serial.print(F(x))
#define	PRINTX(x)	Serial.println(x, HEX)
#else
#define	PRINT(s, x)
#define PRINTS(x)
#define PRINTX(x)
#endif

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may 
// need to be adapted
#define	MAX_DEVICES	8
#define	MAX_ZONES	  2
#define STEP_SIZE   2

#define	CLK_PIN		13
#define	DATA_PIN	11
#define	CS_PIN		10

// Hardware SPI connection
MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);
// Arbitrary output pins
// MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define	SPEED_TIME	25
#define	PAUSE_TIME	1000

// Global variables
char	*pc[MAX_ZONES] = 
{ 
  "Zone0",
  "Zone1",
};

void setZones(void)
{
  static uint8_t zoneBoundary = 0;

  PRINTS("\nZones");
  if (zoneBoundary != 0)
  {
    P.setZone(0, 0, zoneBoundary-1);
    PRINT(" [0] 0:", zoneBoundary - 1);
    P.displayZoneText(0, pc[0], CENTER, SPEED_TIME, PAUSE_TIME, SCROLL_LEFT, SCROLL_LEFT);
    P.displayReset(0);
  }

  if (zoneBoundary != MAX_DEVICES)
  {
    P.setZone(1, zoneBoundary, MAX_DEVICES-1);
    PRINT(" [1] ", zoneBoundary);
    PRINT(":", MAX_DEVICES-1);
    P.displayZoneText(1, pc[1], CENTER, SPEED_TIME, PAUSE_TIME, SCROLL_LEFT, SCROLL_LEFT);
    P.displayReset(1);
  }

  // Set new zone sizes
  zoneBoundary += STEP_SIZE;
  if (zoneBoundary > MAX_DEVICES) zoneBoundary = 0;

  P.synchZoneStart();
}

void setup(void)
{
#if DEBUG
  Serial.begin(57600);
#endif
  PRINTS("[Parola Dynamic Zone Demo]");

  P.begin(MAX_ZONES);
  P.setInvert(false);
  
  setZones(); 
}

void loop(void)
{
  if (P.displayAnimate()) // animates and returns true when an animation is completed
  {
    boolean bAllDone = true;

    for (uint8_t i=0; i<MAX_ZONES && bAllDone; i++)
    {
      bAllDone = bAllDone && P.getZoneStatus(i);
    }

    if (bAllDone) setZones();
  }
}
