// Program to demonstrate the MD_Parola library
//
// Iterate through all combinations of entry and exit effects 
// in 2 zones - one in normal mode and the second in inverted 
// mirrored mode.

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#if USE_LIBRARY_SPI
#include <SPI.h>
#endif

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may 
// need to be adapted
#define	MAX_DEVICES	8
#define	MAX_ZONES	2

#define	CLK_PIN		13
#define	DATA_PIN	11
#define	CS_PIN		10

// Hardware SPI connection
MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);
// Arbitrary output pins
// MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define	SPEED_TIME	25
#define	PAUSE_TIME	1000

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

// Global variables
#define ARRAY_SIZE(x)  (sizeof(x)/sizeof(x[0]))

// Global data
typedef struct
{
  textEffect_t  effect;   // text effect to display
  char *        psz;      // text string nul terminated
  uint16_t      speed;    // speed multiplier of library default
  uint16_t      pause;    // pause multiplier for library default
} sCatalog;

sCatalog  catalog[] =
{
  { PRINT,              "PRINT", 1, 1 },
//  { SLICE,              "SLICE", 1, 1 },
	{ MESH,								"MESH", 20, 1 },
  { WIPE,               "WIPE",  3, 1 },
  { WIPE_CURSOR,        "WPE_C", 4, 1 },
  { OPENING,            "OPEN",  3, 1 },
  { OPENING_CURSOR,     "OPN_C", 4, 1 },
  { CLOSING,            "CLOSE", 3, 1 },
  { CLOSING_CURSOR,     "CLS_C", 4, 1 },
  { BLINDS,             "BLIND", 7, 1 },
  { DISSOLVE,           "DSLVE", 7, 1 },
  { SCROLL_UP,          "SC_U",  5, 1 },
  { SCROLL_DOWN,        "SC_D",  5, 1 },
//  { SCROLL_LEFT,        "SC_L",  5, 1 },
//  { SCROLL_RIGHT,       "SC_R",  5, 1 },
  { SCROLL_UP_LEFT,     "SC_UL", 7, 1 },
  { SCROLL_UP_RIGHT,    "SC_UR", 7, 1 },
  { SCROLL_DOWN_LEFT,   "SC_DL", 7, 1 },
  { SCROLL_DOWN_RIGHT,  "SC_DR", 7, 1 },
  { SCAN_HORIZ,         "SCANH", 4, 1 },
  { SCAN_VERT,          "SCANV", 3, 1 },
  { GROW_UP,            "GRW_U", 7, 1 },
  { GROW_DOWN,          "GRW_D", 7, 1 },
};

void setup(void)
{
#if DEBUG
  Serial.begin(57600);
  PRINTS("[Parola Zone Mirror]");
#endif

  P.begin(MAX_ZONES);
  P.setInvert(false);
  
  for (uint8_t i=0; i<ARRAY_SIZE(catalog); i++)
  {
    catalog[i].speed *= P.getSpeed();     // use the library defaults as multiplier
    catalog[i].pause *= 500;
  }

  P.setZone(0, 0, (MAX_DEVICES/2) - 1);
  P.setZone(1, MAX_DEVICES/2, MAX_DEVICES-1); 
  
  P.setZoneEffect(1, true, FLIP_UD);
  P.setZoneEffect(1, true, FLIP_LR);
}

void loop(void)
{
  static uint8_t  nCurIdx = 0;
  
  // animates and returns true when an animation is completed. These are synchronised, so assume they are all done.
  if (P.displayAnimate()) 
  {
    for (uint8_t z=0; z<MAX_ZONES; z++)
      P.displayZoneText(z, catalog[nCurIdx].psz, CENTER, catalog[nCurIdx].speed, catalog[nCurIdx].pause, catalog[nCurIdx].effect, catalog[nCurIdx].effect);

    nCurIdx = (nCurIdx + 1) % ARRAY_SIZE(catalog);
  }
}
