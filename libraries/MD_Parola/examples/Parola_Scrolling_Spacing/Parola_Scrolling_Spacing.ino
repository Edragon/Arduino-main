// Use the Parola library to scroll text on the display
//
// Demonstrates the use of the scrolling function to display text received 
// from the serial interface and how to adjust the spacing between the end
// of one message at the start of the following one.
//
// User can enter text on the serial monitor and this will display as a
// scrolling message on the display.
// Spacing for the display is controlled by a pot on SPACE_IN analog in.
// Scrolling direction is controlled by a switch on DIRECTION_SET digital in.
// Invert ON/OFF is set by a switch on INVERT_SET digital in.
//
// Keyswitch library can be found at http://arduinocode.codeplex.com
//

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#if USE_LIBRARY_SPI
#include <SPI.h>
#endif

// set to 1 if we are implementing the user interface pot, switch, etc
#include <MD_KeySwitch.h>

// Turn on debug statements to the serial output
#define  DEBUG  1

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
#define	CLK_PIN		13
#define	DATA_PIN	11
#define	CS_PIN		10

// HARDWARE SPI
MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);
// SOFTWARE SPI
//MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define	PAUSE_TIME		500
#define FRAME_TIME    50
#define SPACE_DEADBAND  2

// Scrolling parameters
#define	SPACE_IN		  A5
#define	DIRECTION_SET	8	// change the effect
#define	INVERT_SET		9	// change the invert

textEffect_t	scrollEffect = SCROLL_LEFT;

// Global message buffers shared by Serial and Scrolling functions
#define	BUF_SIZE	75
char curMessage[BUF_SIZE];
char newMessage[BUF_SIZE];
bool newMessageAvailable = false;

MD_KeySwitch uiDirection(DIRECTION_SET);
MD_KeySwitch uiInvert(INVERT_SET);

void doUI(void)
{
  // SPACING
  {
    uint16_t	space = map(analogRead(SPACE_IN), 0, 1023, 0, (MAX_DEVICES+1)*COL_SIZE);
    
    if (space != P.getScrollSpacing())
    {
      P.setScrollSpacing(space);
      PRINT("\nChanged spacing to ", P.getScrollSpacing());
    }
  }

  if (uiDirection.read() == MD_KeySwitch::KS_PRESS)	// SCROLL DIRECTION
  {
    PRINTS("\nChanging scroll direction");
    scrollEffect = (scrollEffect == SCROLL_LEFT ? SCROLL_RIGHT : SCROLL_LEFT);
    P.setTextEffect(scrollEffect, scrollEffect);
    P.displayReset();
  }

  if (uiInvert.read() == MD_KeySwitch::KS_PRESS)	// INVERT MODE
  {
    PRINTS("\nChanging invert mode");
    P.setInvert(!P.getInvert());
  }
}

void readSerial(void)
{
  static uint8_t	putIndex = 0;

  while (Serial.available())
  {
    newMessage[putIndex] = (char)Serial.read();
    if ((newMessage[putIndex] == '\n') || (putIndex >= BUF_SIZE-2))	// end of message character or full buffer
    {
      // put in a message separator and end the string
      newMessage[putIndex] = '\0';
      // restart the index for next filling spree and flag we have a message waiting
      putIndex = 0;
      newMessageAvailable = true;
    }
      else
      // Just save the next char in next location
      newMessage[putIndex++];
  }
}

void setup()
{
  Serial.begin(57600);

  uiDirection.begin();
  uiInvert.begin();
  pinMode(SPACE_IN, INPUT);

  doUI();

  P.begin();
  P.displayClear();
  P.displaySuspend(false);
  P.displayText(curMessage, CENTER, FRAME_TIME, PAUSE_TIME, scrollEffect, scrollEffect);

  strcpy(curMessage, "Hi! Enter new message?");
  newMessage[0] = '\0';

  Serial.begin(57600);
  Serial.print("\n[Parola Scrolling Spacing]\nType a message for the scrolling display\nEnd message line with a newline");
}

void loop() 
{
	doUI();

  readSerial();
  if (P.displayAnimate()) 
  {
    if (newMessageAvailable)
    {
      strcpy(curMessage, newMessage);
      newMessageAvailable = false;
    }
    P.displayReset();
  }
}

