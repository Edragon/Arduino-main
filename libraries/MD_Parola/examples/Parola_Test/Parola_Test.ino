// Program to exercise the MD_Parola library
// 
// Demonstrates most of the functions of the Parola library.
// All animations can be run and tested under user control.
//
// Speed for the display is controlled by a pot on SPEED_IN analog input.
// Digital switches used for control of Justification, Effect progression, 
// Pause between animations, LED intensity, Display flip, and invert mode. 
// UI switches are normally HIGH.
//
// Keyswitch library can be found at http://arduinocode.codeplex.com
//

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#if USE_LIBRARY_SPI
#include <SPI.h>
#endif
#include <MD_KeySwitch.h>

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

// Turn on debug statements to the serial output
#define  DEBUG_ENABLE  1

#if  DEBUG_ENABLE
#define	DEBUG(s, x)	{ Serial.print(F(s)); Serial.print(x); }
#define	DEBUGS(x)	Serial.print(F(x))
#define	DEBUGX(x)	Serial.println(x, HEX)
#else
#define	DEBUG(s, x)
#define DEBUGS(x)
#define DEBUGX(x)
#endif

// User interface pin and switch definitions
#define	SPEED_IN	A5	// control the speed with an external pot
#define	PAUSE_SET	4	// toggle pause time
#define FLIP_SET  5 // toggle flip status
#define JUSTIFY_SET	6	// change the justification
#define	INTENSITY_SET	7	// change the intensity of the display
#define	EFFECT_SET	8	// change the effect
#define	INVERSE_SET	9	// set/reset the display to inverse

#define	PAUSE_TIME		1000	// in milliseconds
#define	SPEED_DEADBAND	5		// in analog units

// Global variables
uint8_t	curString = 0;
char	*pc[] = 
{ 
  "Parola for",
  "Arduino", 
  "LED Matrix",
  "Display" 
};
#define	NEXT_STRING	((curString + 1) % ARRAY_SIZE(pc))

MD_KeySwitch uiJustify(JUSTIFY_SET);
MD_KeySwitch uiEffect(EFFECT_SET);
MD_KeySwitch uiPause(PAUSE_SET);
MD_KeySwitch uiIntensity(INTENSITY_SET);
MD_KeySwitch uiInverse(INVERSE_SET);
MD_KeySwitch uiFlip(FLIP_SET);

void doUI(void)
{
  // set the speed if it has changed
  {
    int16_t	speed = map(analogRead(SPEED_IN), 0, 1023, 0, 100);

    if ((speed >= ((int16_t)P.getSpeed() + SPEED_DEADBAND)) || 
      (speed <= ((int16_t)P.getSpeed() - SPEED_DEADBAND)))
    {
      P.setSpeed(speed);
      DEBUG("\nChanged speed to ", P.getSpeed());
    }
  }

  // now process the digital inputs
  if (uiJustify.read() == MD_KeySwitch::KS_PRESS)	// TEXT ALIGNMENT
  {
    static uint8_t	curMode = 0;
    textPosition_t	align = P.getTextAlignment();
    textPosition_t	textAlign[] = 
    { 
      LEFT, 
      CENTER, 
      RIGHT
    };

    DEBUG("\nChanging alignment to ", curMode);
    P.setTextAlignment(textAlign[curMode]);
    P.displayReset();
    curMode = (curMode + 1) % ARRAY_SIZE(textAlign);
  }

  if (uiEffect.read() == MD_KeySwitch::KS_PRESS)	// EFFECT CHANGE
  {
    static uint8_t  curFX = 0;

    textEffect_t effect[] =
    {
      PRINT,
      SCROLL_UP,
      SCROLL_DOWN,
      SCROLL_LEFT,
      SCROLL_RIGHT,
#if ENA_MISC
      SLICE,
      FADE,
      MESH,
      BLINDS,
      DISSOLVE,
#endif
#if ENA_WIPE
      WIPE,
      WIPE_CURSOR,
#endif
#if ENA_OPNCLS
      OPENING,
      OPENING_CURSOR,
      CLOSING,
      CLOSING_CURSOR,
#endif
#if ENA_SCR_DIA
      SCROLL_UP_LEFT,
      SCROLL_UP_RIGHT,
      SCROLL_DOWN_LEFT,
      SCROLL_DOWN_RIGHT,
#endif
#if ENA_SCAN
      SCAN_HORIZ,
      SCAN_VERT,
#endif
#if ENA_GROW
      GROW_UP,
      GROW_DOWN,
#endif
   };

    curFX = (curFX + 1) % ARRAY_SIZE(effect);
    DEBUG("\nChanging effect to ", curFX);
    P.setTextEffect(effect[curFX], effect[curFX]);
    P.displayReset();
  }

  if (uiPause.read() == MD_KeySwitch::KS_PRESS)	// PAUSE DELAY
  {
    DEBUGS("\nChanging pause");
    if (P.getPause() <= P.getSpeed())
      P.setPause(PAUSE_TIME);
    else
      P.setPause(0);
  }

  if (uiIntensity.read() == MD_KeySwitch::KS_PRESS)	// INTENSITY
  {
    static uint8_t	intensity = 7;

    intensity = ++intensity % (MAX_INTENSITY + 1);
    P.setIntensity(intensity);
    DEBUG("\nChanged intensity to ", intensity);
  }

  if (uiInverse.read() == MD_KeySwitch::KS_PRESS)		// INVERSE
  {
    P.setInvert(!P.getInvert());
  }

  if (uiFlip.read() == MD_KeySwitch::KS_PRESS)      // FLIP
  {
    P.setZoneEffect(0, !P.getZoneEffect(0, FLIP_LR), FLIP_LR);
    P.setZoneEffect(0, !P.getZoneEffect(0, FLIP_UD), FLIP_UD);
  }
}

void setup(void)
{
#if DEBUG_ENABLE
  Serial.begin(57600);
  DEBUGS("[Parola Test]");
#endif

  // user interface switches
  uiJustify.begin();
  uiEffect.begin();
  uiPause.begin();
  uiIntensity.begin();
  uiInverse.begin();
  uiFlip.begin();

  // parola object
  P.begin();
  P.displayText(pc[curString], CENTER, P.getSpeed(), PAUSE_TIME, PRINT, PRINT);
  curString = NEXT_STRING;
}

void loop(void)
{
  doUI();

  if (P.displayAnimate()) 
  {
    P.setTextBuffer(pc[curString]);
    P.displayReset();
    curString = NEXT_STRING;
  }
}


