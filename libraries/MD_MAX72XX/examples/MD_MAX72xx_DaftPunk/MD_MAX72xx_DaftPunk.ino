// Use the MD_MAX72XX library to Display a Daft Punk LED Helmet
//
// If RUN_DEMO is set to zero the display cycles changes triggered by a switch on
// the MODE_SWITCH pin. This can be substituted for any trigger as implemented 
// by the helmet wearer. 
// If RUN_DEMO is set to 1 the sketch will cycle each element of the display every 
// DEMO_DELAY seconds, without the need for a switch.
//
// Uses the MD_Keyswitch library found at http://arduinocode.codeplex.com/releases

#define RUN_DEMO  1

#include <MD_MAX72xx.h>
#if RUN_DEMO
#define DEMO_DELAY  15  // time to show each demo element in seconds
#else
#include <MD_KeySwitch.h>
#endif

#define	DEBUG	0		// Enable or disable (default) debugging output

#if DEBUG
#define	PRINT(s, v)		{ Serial.print(F(s)); Serial.print(v); }		  // Print a string followed by a value (decimal)
#define	PRINTX(s, v)	{ Serial.print(F(s)); Serial.print(v, HEX); }	// Print a string followed by a value (hex)
#define	PRINTB(s, v)	{ Serial.print(F(s)); Serial.print(v, BIN); }	// Print a string followed by a value (binary)
#define PRINTC(s, v)  { Serial.print(F(s)); Serial.print((char)v); }	// Print a string followed by a value (char)
#define	PRINTS(s)		  { Serial.print(F(s)); }							          // Print a string
#else
#define	PRINT(s, v)		// Print a string followed by a value (decimal)
#define	PRINTX(s, v)	// Print a string followed by a value (hex)
#define	PRINTB(s, v)	// Print a string followed by a value (binary)
#define PRINTC(s, v)  // Print a string followed by a value (char)
#define	PRINTS(s)		  // Print a string
#endif

// --------------------
// MD_MAX72xx hardware definitions and object
// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may 
// need to be adapted
//
#define	MAX_DEVICES	5
#define	CLK_PIN		13  // or SCK
#define	DATA_PIN	11  // or MOSI
#define	CS_PIN		10  // or SS

MD_MAX72XX mx = MD_MAX72XX(CS_PIN, MAX_DEVICES);                      // SPI hardware interface
//MD_MAX72XX mx = MD_MAX72XX(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES); // Arbitrary pins

#if !RUN_DEMO
// --------------------
// Mode keyswitch parameters and object
//
#define MODE_SWITCH 9 // Digital Pin

MD_KeySwitch  ks = MD_KeySwitch(MODE_SWITCH, LOW);
#endif

// --------------------
// Constant parameters
//
// Various delays in milliseconds
#define UNIT_DELAY      25
#define SCROLL_DELAY	  (4 * UNIT_DELAY)
#define MIDLINE_DELAY	  (6 * UNIT_DELAY)
#define SCANNER_DELAY   (2 * UNIT_DELAY) 
#define RANDOM_DELAY    (6 * UNIT_DELAY) 
#define FADE_DELAY      (8 * UNIT_DELAY)
#define SPECTRUM_DELAY  (4 * UNIT_DELAY)
#define HEARTBEAT_DELAY (1 * UNIT_DELAY)
#define HEARTS_DELAY    (28 * UNIT_DELAY)
#define EYES_DELAY      (20 * UNIT_DELAY)
#define WIPER_DELAY     (1 * UNIT_DELAY)
#define ARROW_DELAY     (3 * UNIT_DELAY)
#define INVADER_DELAY   (6 * UNIT_DELAY)   

#define	CHAR_SPACING	  1   // pixels between characters
#define	BUF_SIZE	      75  // character buffer size


// ========== General Variables ===========
//
uint32_t prevTimeAnim = 0;    // Used for remembering the millis() value in animations
#if RUN_DEMO
uint32_t prevTimeDemo = 0;      //  Used for remembering the millis() time in demo loop
uint8_t  timeDemo = DEMO_DELAY; // number of seconds left in this demo loop
#endif

// ========== Text routines ===========
//
// Text Message Table
// To change messages simply reorder, add to, or delete from, this table
char *msgTab[] = 
{
  "DAFT PUNK", 
  "GET LUCKY", 
  "ONE MORE TIME",
  "HARDER  BETTER  FASTER  STRONGER",
  "HUMAN AND ROBOT",
  "TECHNOLOGIC",
};

bool scrollText(bool bInit, char *pmsg)
// Callback function for data that is required for scrolling into the display
{
  static char		curMessage[BUF_SIZE];
  static char		*p = curMessage;
  static uint8_t	state = 0;
  static uint8_t	curLen, showLen;
  static uint8_t	cBuf[8];
  uint8_t         colData;

  // are we initializing?
  if (bInit)
  {
    PRINTS("\n--- Initializing ScrollText");
    resetMatrix();
    strcpy(curMessage, pmsg);
    state = 0;
    p = curMessage;
    bInit = false;
  }

  // Is it time to scroll the text?
  if (millis()-prevTimeAnim < SCROLL_DELAY)
    return(bInit);

  // scroll the display
  mx.transform(MD_MAX72XX::TSL);	// scroll along
  prevTimeAnim = millis();			// starting point for next time

  // now run the finite state machine to control what we do
  PRINT("\nScroll FSM S:", state);
  switch (state)
  {
    case 0:	// Load the next character from the font table
      PRINTC("\nLoading ", *p);
      showLen = mx.getChar(*p++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
      curLen = 0;
      state = 1;

    // !! deliberately fall through to next state to start displaying

    case 1:	// display the next part of the character
      colData = cBuf[curLen++];
      mx.setColumn(0, colData);
      if (curLen == showLen)
      {
        showLen = ((*p != '\0') ? CHAR_SPACING : mx.getColumnCount()-1);
        curLen = 0;
        state = 2;
      }
      break;

    case 2:	// display inter-character spacing (blank column) or scroll off the display
      mx.setColumn(0, 0);
      if (++curLen == showLen) 
      {
        state = 0;
        bInit = (*p == '\0');
      }
      break;

    default:
      state = 0;
  }

  return(bInit);
}

// ========== Graphic routines ===========
//
bool graphicMidline1(bool bInit)
{
  // are we initializing?
  if (bInit)
  {
    PRINTS("\n--- Midline1 init");
    resetMatrix();
    bInit = false;
  }
  else
  {
    mx.drawLine(3, 0, 3, mx.getColumnCount()-1, true);
    mx.drawLine(4, 0, 4, mx.getColumnCount()-1, true);
  }

  return(bInit);
}

bool graphicMidline2(bool bInit)
{
  static uint8_t  idx = 0;      // position
  static int8_t   idOffs = 1;   // increment direction

  // are we initializing?
  if (bInit)
  {
    PRINTS("\n--- Midline2 init");
    resetMatrix();
    idx = 0;
    idOffs = 1;
    bInit = false;
  }

  // Is it time to animate?
  if (millis()-prevTimeAnim < MIDLINE_DELAY)
    return(bInit);
  prevTimeAnim = millis();			// starting point for next time

  PRINT("\nML2 R:", idx);
  PRINT(" D:", idOffs);

  // now run the animation
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

  // turn off the old lines
  mx.drawLine(idx, 0, idx, mx.getColumnCount()-1, false);
  mx.drawLine(ROW_SIZE-1-idx, 0, ROW_SIZE-1-idx, mx.getColumnCount()-1, false);

  idx += idOffs;
  if ((idx == 0) || (idx == ROW_SIZE-1)) 
    idOffs = -idOffs;

  // turn on the new lines
  mx.drawLine(idx, 0, idx, mx.getColumnCount()-1, true);
  mx.drawLine(ROW_SIZE-1-idx, 0, ROW_SIZE-1-idx, mx.getColumnCount()-1, true);

  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

  return(bInit);
}

bool graphicScanner(bool bInit)
{
  const uint8_t width = 3;      // scanning bar width
  static uint8_t  idx = 0;      // position
  static int8_t   idOffs = 1;   // increment direction

  // are we initializing?
  if (bInit)
  {
    PRINTS("\n--- Scanner init");
    resetMatrix();
    idx = 0;
    idOffs = 1;
    bInit = false;
  }

  // Is it time to animate?
  if (millis()-prevTimeAnim < SCANNER_DELAY)
    return(bInit);
  prevTimeAnim = millis();			// starting point for next time

  PRINT("\nS R:", idx);
  PRINT(" D:", idOffs);

  // now run the animation
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

  // turn off the old lines
  for (uint8_t i=0; i<width; i++)
    mx.setColumn(idx+i, 0);

  idx += idOffs;
  if ((idx == 0) || (idx + width == mx.getColumnCount())) 
    idOffs = -idOffs;

  // turn on the new lines
  for (uint8_t i=0; i<width; i++)
    mx.setColumn(idx+i, 0xff);

  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

  return(bInit);
}

bool graphicRandom(bool bInit)
{
  // are we initializing?
  if (bInit)
  {
    PRINTS("\n--- Random init");
    resetMatrix();
    bInit = false;
  }

  // Is it time to animate?
  if (millis()-prevTimeAnim < RANDOM_DELAY)
    return(bInit);
  prevTimeAnim = millis();			// starting point for next time

  // now run the animation
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  for (uint8_t i=0; i<mx.getColumnCount(); i++)
    mx.setColumn(i, (uint8_t)random(255));
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

  return(bInit);
}

bool graphicScroller(bool bInit)
{
  const uint8_t   width = 3;     // width of the scroll bar
  const uint8_t   offset = mx.getColumnCount()/3;
  static uint8_t  idx = 0;      // counter

  // are we initializing?
  if (bInit)
  {
    PRINTS("\n--- Scroller init");
    resetMatrix();
    idx = 0;
    bInit = false;
  }

  // Is it time to animate?
  if (millis()-prevTimeAnim < SCANNER_DELAY)
    return(bInit);
  prevTimeAnim = millis();			// starting point for next time

  PRINT("\nS I:", idx);

  // now run the animation
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

  mx.transform(MD_MAX72XX::TSL);

  mx.setColumn(0, idx>=0 && idx<width ? 0xff : 0);
  if (++idx == offset) idx = 0;

  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

  return(bInit);
}

bool graphicSpectrum(bool bInit)
{
  // are we initializing?
  if (bInit)
  {
    PRINTS("\n--- Spectrum init");
    resetMatrix();
    bInit = false;
  }

  // Is it time to animate?
  if (millis()-prevTimeAnim < SPECTRUM_DELAY)
    return(bInit);
  prevTimeAnim = millis();			// starting point for next time

  // now run the animation
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  for (uint8_t i=0; i<MAX_DEVICES; i++)
  {
    uint8_t r = random(ROW_SIZE);
    uint8_t cd = 0; 
    
    for (uint8_t j=0; j<r; j++)
      cd |= 1<<j;
    for (uint8_t j=1; j<COL_SIZE-1; j++)
      mx.setColumn(i, j, ~cd);
  }
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

  return(bInit);
}

bool graphicHeartbeat(bool bInit)
{
  #define BASELINE_ROW  4

  static uint8_t  state;
  static uint8_t  r, c;
  static bool     bPoint;

  // are we initializing?
  if (bInit)
  {
    PRINTS("\n--- Heartbeat init");
    resetMatrix();
    state = 0;
    r = BASELINE_ROW;
    c = mx.getColumnCount()-1;
    bPoint = true;
    bInit = false;
  }

  // Is it time to animate?
  if (millis()-prevTimeAnim < HEARTBEAT_DELAY)
    return(bInit);
  prevTimeAnim = millis();			// starting point for next time

  // now run the animation
  PRINT("\nHB S:", state);
  PRINT(" R: ", r);
  PRINT(" C: ", c);
  PRINT(" P: ", bPoint);
  mx.setPoint(r, c, bPoint);

  switch (state)
  {
    case 0: // straight line from the right side
      if (c == mx.getColumnCount()/2 + COL_SIZE)
        state = 1;
      c--;
      break;

    case 1: // first stroke 
      if (r != 0) { r--; c--; }
      else state = 2;      
      break;

    case 2: // down stroke
      if (r != ROW_SIZE-1) { r++; c--; }
      else state = 3;
      break;

    case 3: // second up stroke
      if (r != BASELINE_ROW) { r--; c--; }
      else state = 4;
      break;

    case 4: // straight line to the left
      if (c == 0) 
      {
        c = mx.getColumnCount()-1;
        bPoint = !bPoint;
        state = 0;
      }
      else c--;
      break;

    default:
      state = 0;
  }

  return(bInit);
}

bool graphicFade(bool bInit)
{
  static uint8_t  intensity = 0;
  static int8_t   iOffs = 1;

  // are we initializing?
  if (bInit)
  {
    PRINTS("\n--- Fade init");
    resetMatrix();
    mx.control(MD_MAX72XX::INTENSITY, intensity);

    // Set all LEDS on
    mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    for (uint8_t i=0; i<mx.getColumnCount(); i++)
      mx.setColumn(i, 0xff);
    mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

    bInit = false;
  }

  // Is it time to animate?
  if (millis()-prevTimeAnim < FADE_DELAY)
    return(bInit);
  prevTimeAnim = millis();			// starting point for next time

  // now run the animation
  intensity += iOffs;
  PRINT("\nF I:", intensity);
  PRINT(" D:", iOffs);
  if ((intensity == 0) || (intensity == MAX_INTENSITY))
    iOffs = -iOffs;
  mx.control(MD_MAX72XX::INTENSITY, intensity);

  return(bInit);
}

bool graphicHearts(bool bInit)
{
#define NUM_HEARTS  ((MAX_DEVICES/2) +1)
  const uint8_t heartFull[] = { 0x1c, 0x3e, 0x7e, 0xfc };
  const uint8_t heartEmpty[] = { 0x1c, 0x22, 0x42, 0x84 };
  const uint8_t offset = mx.getColumnCount()/(NUM_HEARTS+1);
  const uint8_t dataSize = (sizeof(heartFull)/sizeof(heartFull[0]));

  static bool     bEmpty;
  static uint8_t  r, c;

  // are we initializing?
  if (bInit)
  {
    PRINTS("\n--- Hearts init");
    resetMatrix();
    bEmpty = true;
    bInit = false;
  }

  // Is it time to animate?
  if (millis()-prevTimeAnim < HEARTS_DELAY)
    return(bInit);
  prevTimeAnim = millis();			// starting point for next time

  // now run the animation
  PRINT("\nH E:", bEmpty);

  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  for (uint8_t h=1; h<=NUM_HEARTS; h++)
  {
    for (uint8_t i=0; i<dataSize; i++)
    {
      mx.setColumn((h*offset)-dataSize+i, bEmpty ? heartEmpty[i] : heartFull[i]);
      mx.setColumn((h*offset)+dataSize-i-1, bEmpty ? heartEmpty[i] : heartFull[i]);
    }
  }
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  bEmpty = !bEmpty;

  return(bInit);
}

bool graphicEyes(bool bInit)
{
#define NUM_EYES  2
  const uint8_t eyeOpen[] = { 0x18, 0x3c, 0x66, 0x66 };
  const uint8_t eyeClose[] = { 0x18, 0x3c, 0x3c, 0x3c };
  const uint8_t offset = mx.getColumnCount()/(NUM_EYES+1);
  const uint8_t dataSize = (sizeof(eyeOpen)/sizeof(eyeOpen[0]));

  bool bOpen;

  // are we initializing?
  if (bInit)
  {
    PRINTS("\n--- Eyes init");
    resetMatrix();
    bInit = false;
  }

  // Is it time to animate?
  if (millis()-prevTimeAnim < EYES_DELAY)
    return(bInit);
  prevTimeAnim = millis();			// starting point for next time

  // now run the animation
  bOpen = (random(1000) > 100);
  PRINT("\nH E:", bOpen);

  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  for (uint8_t e=1; e<=NUM_EYES; e++)
  {
    for (uint8_t i=0; i<dataSize; i++)
    {
      mx.setColumn((e*offset)-dataSize+i, bOpen ? eyeOpen[i] : eyeClose[i]);
      mx.setColumn((e*offset)+dataSize-i-1, bOpen ? eyeOpen[i] : eyeClose[i]);
    }
  }
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

  return(bInit);
}

bool graphicBounceBall(bool bInit)
{
  static uint8_t  idx = 0;      // position
  static int8_t   idOffs = 1;   // increment direction

  // are we initializing?
  if (bInit)
  {
    PRINTS("\n--- BounceBall init");
    resetMatrix();
    bInit = false;
  }

  // Is it time to animate?
  if (millis()-prevTimeAnim < SCANNER_DELAY)
    return(bInit);
  prevTimeAnim = millis();			// starting point for next time

  PRINT("\nBB R:", idx);
  PRINT(" D:", idOffs);

  // now run the animation
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

  // turn off the old ball
  mx.setColumn(idx, 0);
  mx.setColumn(idx+1, 0);

  idx += idOffs;
  if ((idx == 0) || (idx == mx.getColumnCount()-2)) 
    idOffs = -idOffs;

  // turn on the new lines
  mx.setColumn(idx, 0x18);
  mx.setColumn(idx+1, 0x18);

  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

  return(bInit);
}

bool graphicArrow(bool bInit)
{
  const uint8_t arrow[] = { 0x3c, 0x66, 0xc3, 0x99 };
  const uint8_t dataSize = (sizeof(arrow)/sizeof(arrow[0]));

  static uint8_t  idx = 0;

  // are we initializing?
  if (bInit)
  {
    PRINTS("\n--- Arrow init");
    resetMatrix();
    bInit = false;
  }

  // Is it time to animate?
  if (millis()-prevTimeAnim < ARROW_DELAY)
    return(bInit);
  prevTimeAnim = millis();			// starting point for next time

  // now run the animation
  PRINT("\nAR I:", idx);

  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  mx.transform(MD_MAX72XX::TSL);
  mx.setColumn(0, arrow[idx++]);
  if (idx == dataSize) idx = 0;
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

  return(bInit);
}

bool graphicWiper(bool bInit)
{
  static uint8_t  idx = 0;      // position
  static int8_t   idOffs = 1;   // increment direction

  // are we initializing?
  if (bInit)
  {
    PRINTS("\n--- Wiper init");
    resetMatrix();
    bInit = false;
  }

  // Is it time to animate?
  if (millis()-prevTimeAnim < WIPER_DELAY)
    return(bInit);
  prevTimeAnim = millis();			// starting point for next time

  PRINT("\nW R:", idx);
  PRINT(" D:", idOffs);

  // now run the animation
  mx.setColumn(idx, idOffs == 1 ? 0xff : 0);
  idx += idOffs;
  if ((idx == 0) || (idx == mx.getColumnCount())) 
    idOffs = -idOffs;

  return(bInit);
}

bool graphicInvader(bool bInit)
{
  const uint8_t invader1[] = { 0x0e, 0x98, 0x7d, 0x36, 0x3c };
  const uint8_t invader2[] = { 0x70, 0x18, 0x7d, 0xb6, 0x3c };
  const uint8_t dataSize = (sizeof(invader1)/sizeof(invader1[0]));

  static int8_t idx;
  static bool   iType;

  // are we initializing?
  if (bInit)
  {
    PRINTS("\n--- Invader init");
    resetMatrix();
    bInit = false;
    idx = -dataSize;
    iType = false;
  }

  // Is it time to animate?
  if (millis()-prevTimeAnim < INVADER_DELAY)
    return(bInit);
  prevTimeAnim = millis();			// starting point for next time

  // now run the animation
  PRINT("\nINV I:", idx);

  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  mx.clear();
  for (uint8_t i=0; i<dataSize; i++)
  {
    mx.setColumn(idx-dataSize+i, iType ? invader1[i] : invader2[i]);
    mx.setColumn(idx+dataSize-i-1, iType ? invader1[i] : invader2[i]);
  }
  idx++;
  if (idx == mx.getColumnCount()+(dataSize*2)) bInit = true;
  iType = !iType;
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

  return(bInit);
}

// ========== Control routines ===========
//
void resetMatrix(void)
{
  mx.control(MD_MAX72XX::INTENSITY, MAX_INTENSITY/2);
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  mx.clear();
  prevTimeAnim = 0;
}

void runMatrixAnimation(void)
// Schedule the animations, switching to the next one when the 
// the mode switch is pressed.
{
  static  uint8_t state = 0;
  static  uint8_t mesg = 0;
  static  boolean bRestart = true;
  boolean changeState = false;

#if RUN_DEMO
  // check if one second has passed and then count down the demo timer. Once this
  // gets to zero, change the state.
  if (millis()-prevTimeDemo >= 1000)
  {
    prevTimeDemo = millis();
    if (--timeDemo == 0) 
    {
      timeDemo = DEMO_DELAY;
      changeState = true;
    }
  }         
#else
  // check if the switch is pressed and handle that first
  changeState = ks.read();
#endif
  if (changeState)
  {
    if (state == 0) // the message display state
    {
      mesg++;
      if (mesg >= sizeof(msgTab)/sizeof(msgTab[0]))
      {
        mesg = 0;
        state++;
      }
    }
    else
      state++;

    bRestart = true;
  };

  // now do whatever we do in the current state
  switch(state)
  {
    case  0: bRestart = scrollText(bRestart, msgTab[mesg]); break;
    case  1: bRestart = graphicMidline1(bRestart);       break;
    case  2: bRestart = graphicMidline2(bRestart);       break;
    case  3: bRestart = graphicScanner(bRestart);        break;
    case  4: bRestart = graphicRandom(bRestart);         break;
    case  5: bRestart = graphicFade(bRestart);           break;
    case  6: bRestart = graphicSpectrum(bRestart);       break;
    case  7: bRestart = graphicHeartbeat(bRestart);      break;
    case  8: bRestart = graphicHearts(bRestart);         break;
    case  9: bRestart = graphicEyes(bRestart);           break;
    case 10: bRestart = graphicBounceBall(bRestart);     break;
    case 11: bRestart = graphicArrow(bRestart);          break;
    case 12: bRestart = graphicScroller(bRestart);       break;
    case 13: bRestart = graphicWiper(bRestart);          break;
    case 14: bRestart = graphicInvader(bRestart);        break;
   
    default: state = 0;
  }
}

void setup()
{
  mx.begin();
  prevTimeAnim = millis();
#if RUN_DEMO
  prevTimeDemo = millis();
#else
  ks.begin();
#endif
#if DEBUG
  Serial.begin(57600);
#endif
  PRINTS("\n[MD_MAX72XX DaftPunk]");
}

void loop() 
{
  runMatrixAnimation();
  // other code to run the helmet goes here
}

