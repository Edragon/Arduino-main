//Contains the definitions of the functions used by the firmware.


#include <SPI.h>
#include <Wire.h>
#include <FlashStorage.h>

#include "Parameters.h"
#include "Controller.h"
#include "Utils.h"
#include "State.h"
#include "analogFastWrite.h"

void setupPins() {

  pinMode(VREF_2, OUTPUT);
  pinMode(VREF_1, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_1, OUTPUT);

  pinMode(chipSelectPin, OUTPUT); // CSn -- has to toggle high and low to signal chip to start data transfer

  
#ifdef ENABLE_PROFILE_IO  
  pinMode(TEST1, OUTPUT);
#endif

  pinMode(ledPin, OUTPUT); //

  // pinMode(clockPin, OUTPUT); // SCL    for I2C
  // pinMode(inputPin, INPUT); // SDA


  analogFastWrite(VREF_2, 0.33 * uMAX);
  analogFastWrite(VREF_1, 0.33 * uMAX);

  IN_4_HIGH();   //  digitalWrite(IN_4, HIGH);
  IN_3_LOW();    //  digitalWrite(IN_3, LOW);
  IN_2_HIGH();   //  digitalWrite(IN_2, HIGH);
  IN_1_LOW();    //  digitalWrite(IN_1, LOW);
}

void setupSPI() {

  SPISettings settingsA(10000000, MSBFIRST, SPI_MODE1);             ///400000, MSBFIRST, SPI_MODE1);

  SPI.begin();    //AS5047D SPI uses mode=1 (CPOL=0, CPHA=1)
  SerialUSB.println("Beginning SPI communication with AS5047 encoder...");
  delay(1000);
  SPI.beginTransaction(settingsA);

}

void configureStepDir() {
  pinMode(step_pin, INPUT);
  pinMode(dir_pin, INPUT);
  attachInterrupt(step_pin, stepInterrupt, RISING);
  attachInterrupt(dir_pin, dirInterrupt, CHANGE);
}

void configureEnablePin() {
  pinMode(enable_pin, INPUT);
  attachInterrupt(enable_pin, enableInterrupt, CHANGE);
}


void stepInterrupt() {
  if (dir) r += stepangle;
  else r -= stepangle;
}

void dirInterrupt() {
  if (REG_PORT_IN0 & PORT_PA11) dir = false; // check if dir_pin is HIGH
  else dir = true;
}

void enableInterrupt() {            //enable pin interrupt handler
  if (REG_PORT_IN0 & PORT_PA14){   // check if enable_pin is HIGH
    disableTCInterrupts();
    analogFastWrite(VREF_2, 0);  //set phase currents to zero
    analogFastWrite(VREF_1, 0);
    }
  else{
    enableTCInterrupts();    
    }
}

void output(float theta, int effort) {
   int angle_1;
   int angle_2;
   int v_coil_A;
   int v_coil_B;

   int sin_coil_A;
   int sin_coil_B;
   int phase_multiplier = 10 * spr / 4;

  //REG_PORT_OUTCLR0 = PORT_PA09; for debugging/timing

  angle_1 = mod((phase_multiplier * theta) , 3600);   //
  angle_2 = mod((phase_multiplier * theta)+900 , 3600);
  
  sin_coil_A  = sin_1[angle_1];

  sin_coil_B = sin_1[angle_2];

  v_coil_A = ((effort * sin_coil_A) / 1024);
  v_coil_B = ((effort * sin_coil_B) / 1024);

/*    // For debugging phase voltages:
     SerialUSB.print(v_coil_A);
     SerialUSB.print(",");
     SerialUSB.println(v_coil_B);
*/
  analogFastWrite(VREF_1, abs(v_coil_A));
  analogFastWrite(VREF_2, abs(v_coil_B));

  if (v_coil_A >= 0)  {
    IN_2_HIGH();  //REG_PORT_OUTSET0 = PORT_PA21;     //write IN_2 HIGH
    IN_1_LOW();   //REG_PORT_OUTCLR0 = PORT_PA06;     //write IN_1 LOW
  }
  else  {
    IN_2_LOW();   //REG_PORT_OUTCLR0 = PORT_PA21;     //write IN_2 LOW
    IN_1_HIGH();  //REG_PORT_OUTSET0 = PORT_PA06;     //write IN_1 HIGH
  }

  if (v_coil_B >= 0)  {
    IN_4_HIGH();  //REG_PORT_OUTSET0 = PORT_PA20;     //write IN_4 HIGH
    IN_3_LOW();   //REG_PORT_OUTCLR0 = PORT_PA15;     //write IN_3 LOW
  }
  else  {
    IN_4_LOW();     //REG_PORT_OUTCLR0 = PORT_PA20;     //write IN_4 LOW
    IN_3_HIGH();    //REG_PORT_OUTSET0 = PORT_PA15;     //write IN_3 HIGH
  }

}

static FlashClass flash;
static const unsigned page_size = 256; // actual size is 64?
static unsigned page_count;
static const unsigned floats_per_page = page_size / sizeof(float);
static float page[floats_per_page];
static const void * page_ptr;

static void write_page()
{
  if (0 == (0xFFF & (uintptr_t) page_ptr))
  {
    SerialUSB.println();
    SerialUSB.print("0x");
    SerialUSB.print((uintptr_t) page_ptr, HEX);
  } else {
    SerialUSB.print(".");
  }

  flash.erase((const void*) page_ptr, sizeof(page));
  flash.write((const void*) page_ptr, (const void *) page, sizeof(page));
}

static void store_lookup(float lookupAngle)
{
  page[page_count++] = lookupAngle;
  if(page_count != floats_per_page)
    return;

  // we've filled an entire page, write it to the flash
  write_page();

  // reset our counters and increment our flash page
  page_ptr += sizeof(page);
  page_count = 0;
  memset(page, 0, sizeof(page));
}


void calibrate() {   /// this is the calibration routine

  int encoderReading = 0;     //or float?  not sure if we can average for more res?
  int currentencoderReading = 0;
  int lastencoderReading = 0;
  int avg = 10;               //how many readings to average

  int iStart = 0;     //encoder zero position index
  int jStart = 0;
  int stepNo = 0;
  
  int fullStepReadings[spr];
    
  int fullStep = 0;
  int ticks = 0;
  float lookupAngle = 0.0;
  SerialUSB.println("Beginning calibration routine...");

  encoderReading = readEncoder();
  dir = true;
  oneStep();
  delay(500);

  if ((readEncoder() - encoderReading) < 0)   //check which way motor moves when dir = true
  {
    SerialUSB.println("Wired backwards");    // rewiring either phase should fix this.  You may get a false message if you happen to be near the point where the encoder rolls over...
    return;
  }

  while (stepNumber != 0) {       //go to step zero
    if (stepNumber > 0) {
      dir = true;
    }
    else
    {
      dir = false;
    }
    oneStep();
    delay(100);
  }
  dir = true;
  for (int x = 0; x < spr; x++) {     //step through all full step positions, recording their encoder readings

    encoderReading = 0;
    delay(20);                         //moving too fast may not give accurate readings.  Motor needs time to settle after each step.
    lastencoderReading = readEncoder();
        
    for (int reading = 0; reading < avg; reading++) {  //average multple readings at each step
      currentencoderReading = readEncoder();

      if ((currentencoderReading-lastencoderReading)<(-(cpr/2))){
        currentencoderReading += cpr;
      }
      else if ((currentencoderReading-lastencoderReading)>((cpr/2))){
        currentencoderReading -= cpr;
      }
 
      encoderReading += currentencoderReading;
      delay(10);
      lastencoderReading = currentencoderReading;
    }
    encoderReading = encoderReading / avg;
    if (encoderReading>cpr){
      encoderReading-= cpr;
    }
    else if (encoderReading<0){
      encoderReading+= cpr;
    }

    fullStepReadings[x] = encoderReading;
   // SerialUSB.println(fullStepReadings[x], DEC);      //print readings as a sanity check
    if (x % 20 == 0)
    {
      SerialUSB.println();
      SerialUSB.print(100*x/spr);
      SerialUSB.print("% ");
    } else {
      SerialUSB.print('.');
    }
    
    oneStep();
  }
      SerialUSB.println();

 // SerialUSB.println(" ");
 // SerialUSB.println("ticks:");                        //"ticks" represents the number of encoder counts between successive steps... these should be around 82 for a 1.8 degree stepper
 // SerialUSB.println(" ");
  for (int i = 0; i < spr; i++) {
    ticks = fullStepReadings[mod((i + 1), spr)] - fullStepReadings[mod((i), spr)];
    if (ticks < -15000) {
      ticks += cpr;

    }
    else if (ticks > 15000) {
      ticks -= cpr;
    }
   // SerialUSB.println(ticks);

    if (ticks > 1) {                                    //note starting point with iStart,jStart
      for (int j = 0; j < ticks; j++) {
        stepNo = (mod(fullStepReadings[i] + j, cpr));
        // SerialUSB.println(stepNo);
        if (stepNo == 0) {
          iStart = i;
          jStart = j;
        }

      }
    }

    if (ticks < 1) {                                    //note starting point with iStart,jStart
      for (int j = -ticks; j > 0; j--) {
        stepNo = (mod(fullStepReadings[spr - 1 - i] + j, cpr));
        // SerialUSB.println(stepNo);
        if (stepNo == 0) {
          iStart = i;
          jStart = j;
        }

      }
    }

  }

  // The code below generates the lookup table by intepolating between
  // full steps and mapping each encoder count to a calibrated angle
  // The lookup table is too big to store in volatile memory,
  // so we must generate and store it into the flash on the fly

  // begin the write to the calibration table
  page_count = 0;
  page_ptr = (const uint8_t*) lookup;
  SerialUSB.print("Writing to flash 0x");
  SerialUSB.print((uintptr_t) page_ptr, HEX);
  SerialUSB.print(" page size PSZ=");
  SerialUSB.print(NVMCTRL->PARAM.bit.PSZ);

  for (int i = iStart; i < (iStart + spr + 1); i++) {
    ticks = fullStepReadings[mod((i + 1), spr)] - fullStepReadings[mod((i), spr)];

    if (ticks < -15000) {           //check if current interval wraps over encoder's zero positon
      ticks += cpr;
    }
    else if (ticks > 15000) {
      ticks -= cpr;
    }
    //Here we print an interpolated angle corresponding to each encoder count (in order)
    if (ticks > 1) {              //if encoder counts were increasing during cal routine...

      if (i == iStart) { //this is an edge case
        for (int j = jStart; j < ticks; j++) {
	  store_lookup(0.001 * mod(1000 * ((aps * i) + ((aps * j ) / float(ticks))), 360000.0));
        }
      }

      else if (i == (iStart + spr)) { //this is an edge case
        for (int j = 0; j < jStart; j++) {
	  store_lookup(0.001 * mod(1000 * ((aps * i) + ((aps * j ) / float(ticks))), 360000.0));
        }
      }
      else {                        //this is the general case
        for (int j = 0; j < ticks; j++) {
	  store_lookup(0.001 * mod(1000 * ((aps * i) + ((aps * j ) / float(ticks))), 360000.0));
        }
      }
    }

    else if (ticks < 1) {             //similar to above... for case when encoder counts were decreasing during cal routine
      if (i == iStart) {
        for (int j = - ticks; j > (jStart); j--) {
          store_lookup(0.001 * mod(1000 * (aps * (i) + (aps * ((ticks + j)) / float(ticks))), 360000.0));
        }
      }
      else if (i == iStart + spr) {
        for (int j = jStart; j > 0; j--) {
          store_lookup(0.001 * mod(1000 * (aps * (i) + (aps * ((ticks + j)) / float(ticks))), 360000.0));
        }
      }
      else {
        for (int j = - ticks; j > 0; j--) {
          store_lookup(0.001 * mod(1000 * (aps * (i) + (aps * ((ticks + j)) / float(ticks))), 360000.0));
        }
      }

    }


  }

  if (page_count != 0)
	write_page();

  SerialUSB.println(" ");
  SerialUSB.println(" ");
  SerialUSB.println("Calibration complete!");
  SerialUSB.println("The calibration table has been written to non-volatile Flash memory!");
  SerialUSB.println(" ");
  SerialUSB.println(" ");
}


float read_angle()
{
  const int avg = 10;            //average a few readings
  int encoderReading = 0;

  disableTCInterrupts();        //can't use readEncoder while in closed loop

  for (int reading = 0; reading < avg; reading++) {  //average multple readings at each step
    encoderReading += readEncoder();
    delay(10);
    }

  //return encoderReading * (360.0 / 16384.0) / avg;
  return lookup[encoderReading / avg];
}


void serialCheck() {        //Monitors serial for commands.  Must be called in routinely in loop for serial interface to work.

  if (SerialUSB.available()) {

    char inChar = (char)SerialUSB.read();

    switch (inChar) {


      case 'p':             //print
        print_angle();
        break;

      case 's':             //step
        oneStep();
        print_angle();
        break;

      case 'd':             //dir
        if (dir) {
          dir = false;
        }
        else {
          dir = true;
        }
        break;

      case 'w':                //old command
        calibrate();           //cal routine
        break;
        
      case 'c':
        calibrate();           //cal routine
        break;        

      case 'e':
        readEncoderDiagnostics();   //encoder error?
        break;

      case 'y':
        r = (read_angle()+(360.0 * wrap_count));          // hold the current position
        SerialUSB.print("New setpoint ");
        SerialUSB.println(r, 2);
        enableTCInterrupts();      //enable closed loop
        break;

      case 'n':
        disableTCInterrupts();      //disable closed loop
        analogFastWrite(VREF_2, 0);     //set phase currents to zero
        analogFastWrite(VREF_1, 0);                       
        break;

      case 'r':             //new setpoint
        SerialUSB.println("Enter setpoint:");
        while (SerialUSB.available() == 0)  {}
        r = SerialUSB.parseFloat();
        SerialUSB.println(r);
        break;

      case 'x':
        mode = 'x';           //position loop
        break;

      case 'v':
        mode = 'v';           //velocity loop
        break;

      case 't':
        mode = 't';           //torque loop
        break;

      case 'h':               //hybrid mode
        mode = 'h';
        break;

      case 'q':
        parameterQuery();     // prints copy-able parameters
        break;

      case 'a':             //anticogging
        antiCoggingCal();
        break;

      case 'k':
        parameterEditmain();
        break;
        
      case 'g':
        sineGen();
        break;

      case 'm':
        serialMenu();
        break;
        
      case 'j':
        stepResponse();
        break;


      default:
        break;
    }
  }

}


void parameterQuery() {         //print current parameters in a format that can be copied directly in to Parameters.cpp
  SerialUSB.println(' ');
  SerialUSB.println("----Current Parameters-----");
  SerialUSB.println(' ');
  SerialUSB.println(' ');

  SerialUSB.print("volatile float Fs = ");
  SerialUSB.print(Fs, DEC);
  SerialUSB.println(";  //Sample frequency in Hz");
  SerialUSB.println(' ');

  SerialUSB.print("volatile float pKp = ");
  SerialUSB.print(pKp, DEC);
  SerialUSB.println(";      //position mode PID vallues.");
  
  SerialUSB.print("volatile float pKi = ");
  SerialUSB.print(pKi, DEC);
  SerialUSB.println(";");

  SerialUSB.print("volatile float pKd = ");
  SerialUSB.print(pKd, DEC);
  SerialUSB.println(";");
  
  SerialUSB.print("volatile float pLPF = ");
  SerialUSB.print(pLPF, DEC);
  SerialUSB.println(";");

  SerialUSB.println(' ');

  SerialUSB.print("volatile float vKp = ");
  SerialUSB.print(vKp, DEC);
  SerialUSB.println(";      //velocity mode PID vallues.");

  SerialUSB.print("volatile float vKi = ");
  SerialUSB.print(vKi , DEC);
  SerialUSB.println(";");
 // SerialUSB.println(vKi * Fs, DEC);
 // SerialUSB.println(" / Fs;");

  SerialUSB.print("volatile float vKd = ");
  SerialUSB.print(vKd, DEC);
  SerialUSB.println(";");
 // SerialUSB.print(vKd / Fs);
 // SerialUSB.println(" * FS;");
  SerialUSB.print("volatile float vLPF = ");
  SerialUSB.print(vLPF, DEC);
  SerialUSB.println(";");

  SerialUSB.println("");
  SerialUSB.println("//This is the encoder lookup table (created by calibration routine)");
  SerialUSB.println("");
  
  SerialUSB.println("const float __attribute__((__aligned__(256))) lookup[16384] = {");
  for (int i = 0; i < 16384; i++) {
    SerialUSB.print(lookup[i]);
    SerialUSB.print(", ");
  }
  SerialUSB.println("");
  SerialUSB.println("};");

}



void oneStep() {           /////////////////////////////////   oneStep    ///////////////////////////////
  
  if (!dir) {
    stepNumber += 1;
  }
  else {
    stepNumber -= 1;
  }

  //output(1.8 * stepNumber, 64); //updata 1.8 to aps..., second number is control effort
  output(aps * stepNumber, (int)(0.33 * uMAX));
  delay(10);
}

int readEncoder()           //////////////////////////////////////////////////////   READENCODER   ////////////////////////////
{
  long angleTemp;
  
  CHIPSELECT_LOW(); //digitalWrite(chipSelectPin, LOW);

  byte b1 = SPI.transfer(0xFF);
  byte b2 = SPI.transfer(0xFF);

  angleTemp = (((b1 << 8) | b2) & 0B0011111111111111);

  CHIPSELECT_HIGH();   //digitalWrite(chipSelectPin, HIGH);
  return angleTemp;
}



void readEncoderDiagnostics()           //////////////////////////////////////////////////////   READENCODERDIAGNOSTICS   ////////////////////////////
{
  long angleTemp;
  CHIPSELECT_LOW(); //digitalWrite(chipSelectPin, LOW);

  ///////////////////////////////////////////////READ DIAAGC (0x3FFC)
  SerialUSB.println("------------------------------------------------");

  SerialUSB.println("Checking AS5047 diagnostic and error registers");
  SerialUSB.println("See AS5047 datasheet for details");
  SerialUSB.println(" ");
  ;

  SPI.transfer(0xFF);
  SPI.transfer(0xFC);

  CHIPSELECT_HIGH();   //digitalWrite(chipSelectPin, HIGH);
  delay(1);
  CHIPSELECT_LOW();    //digitalWrite(chipSelectPin, LOW);

  byte b1 = SPI.transfer(0xC0);
  byte b2 = SPI.transfer(0x00);

  SerialUSB.print("Check DIAAGC register (0x3FFC) ...  ");
  SerialUSB.println(" ");

  angleTemp = (((b1 << 8) | b2) & 0B1111111111111111);
  SerialUSB.println((angleTemp | 0B1110000000000000000 ), BIN);

  if (angleTemp & (1 << 14))    SerialUSB.println("  Error occurred  ");

  if (angleTemp & (1 << 11))    SerialUSB.println("  MAGH - magnetic field strength too high, set if AGC = 0x00. This indicates the non-linearity error may be increased");

  if (angleTemp & (1 << 10))    SerialUSB.println("  MAGL - magnetic field strength too low, set if AGC = 0xFF. This indicates the output noise of the measured angle may be increased");

  if (angleTemp & (1 << 9))     SerialUSB.println("  COF - CORDIC overflow. This indicates the measured angle is not reliable");

  if (angleTemp & (1 << 8))     SerialUSB.println("  LF - offset compensation completed. At power-up, an internal offset compensation procedure is started, and this bit is set when the procedure is completed");

  if (!((angleTemp & (1 << 14)) | (angleTemp & (1 << 11)) | (angleTemp & (1 << 10)) | (angleTemp & (1 << 9))))  SerialUSB.println("Looks good!");
  SerialUSB.println(" ");


  CHIPSELECT_HIGH();   //digitalWrite(chipSelectPin, HIGH);
  delay(1);
  CHIPSELECT_LOW();    //digitalWrite(chipSelectPin, LOW);

  SPI.transfer(0x40);
  SPI.transfer(0x01);
  CHIPSELECT_HIGH();   //digitalWrite(chipSelectPin, HIGH);

  delay(1);
  CHIPSELECT_LOW();    //digitalWrite(chipSelectPin, LOW);

  b1 = SPI.transfer(0xC0);
  b2 = SPI.transfer(0x00);


  SerialUSB.print("Check ERRFL register (0x0001) ...  ");
  SerialUSB.println(" ");



  angleTemp = (((b1 << 8) | b2) & 0B1111111111111111);
  SerialUSB.println((angleTemp | 0B1110000000000000000 ), BIN);

  if (angleTemp & (1 << 14)) {
    SerialUSB.println("  Error occurred  ");
  }
  if (angleTemp & (1 << 2)) {
    SerialUSB.println("  parity error ");
  }
  if (angleTemp & (1 << 1)) {
    SerialUSB.println("  invalid register  ");
  }
  if (angleTemp & (1 << 0)) {
    SerialUSB.println("  framing error  ");
  }
  if (!((angleTemp & (1 << 14)) | (angleTemp & (1 << 2)) | (angleTemp & (1 << 1)) | (angleTemp & (1 << 0))))  SerialUSB.println("Looks good!");

  SerialUSB.println(" ");

  CHIPSELECT_HIGH();   //digitalWrite(chipSelectPin, HIGH);


  delay(1);

}


void print_angle()                ///////////////////////////////////       PRINT_ANGLE   /////////////////////////////////
{
  SerialUSB.print("stepNumber: ");
  SerialUSB.print(stepNumber, DEC);
  SerialUSB.print(" , ");
//  SerialUSB.print(stepNumber * aps, DEC);
//  SerialUSB.print(" , ");
  SerialUSB.print("Angle: ");
  SerialUSB.print(read_angle(), 2);
  SerialUSB.print(", raw encoder: ");
  SerialUSB.print(readEncoder());
  SerialUSB.println();
}


void receiveEvent(int howMany)
{
  while (1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    SerialUSB.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  SerialUSB.println(x);         // print the integer
  r = 0.1 * ((float)x);
}

int mod(int xMod, int mMod) {
  return (xMod % mMod + mMod) % mMod;
}



void setupTCInterrupts() {  // configure the controller interrupt

  // Enable GCLK for TC4 and TC5 (timer counter input clock)
  GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TC4_TC5));
  while (GCLK->STATUS.bit.SYNCBUSY);

  TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;   // Disable TCx
  WAIT_TC16_REGS_SYNC(TC5)                      // wait for sync

  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;   // Set Timer counter Mode to 16 bits
  WAIT_TC16_REGS_SYNC(TC5)

  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ; // Set TC as normal Normal Frq
  WAIT_TC16_REGS_SYNC(TC5)

  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1;   // Set perscaler
  WAIT_TC16_REGS_SYNC(TC5)

  TC5->COUNT16.CC[0].reg = (int)( round(48000000 / Fs)); //0x3E72; //0x4AF0;
  WAIT_TC16_REGS_SYNC(TC5)

  TC5->COUNT16.INTENSET.reg = 0;              // disable all interrupts
  TC5->COUNT16.INTENSET.bit.OVF = 1;          // enable overfollow
  TC5->COUNT16.INTENSET.bit.MC0 = 1;         // enable compare match to CC0

  NVIC_SetPriority(TC5_IRQn, 1);              //Set interrupt priority

  // Enable InterruptVector
  NVIC_EnableIRQ(TC5_IRQn);

  // Enable TC
  //  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
  //  WAIT_TC16_REGS_SYNC(TC5)
}

void enableTCInterrupts() {   //enables the controller interrupt ("closed loop mode")
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;    //Enable TC5
  WAIT_TC16_REGS_SYNC(TC5)                      //wait for sync
}

void disableTCInterrupts() {  //disables the controller interrupt ("closed loop mode")
  TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;   // Disable TC5
  WAIT_TC16_REGS_SYNC(TC5)                      // wait for sync
}


void antiCoggingCal() {       //This is still under development...  The idea is that we can calibrate out the stepper motor's detent torque by measuring the torque required to hold all possible positions.
  SerialUSB.println(" -----------------BEGIN ANTICOGGING CALIBRATION!----------------");
  mode = 'x';
  r = lookup[1];
  enableTCInterrupts();
  delay(1000);


  for (int i = 1; i < 657; i++) {
    r = lookup[i];
    SerialUSB.print(r, DEC);
    SerialUSB.print(" , ");
    delay(100);
    SerialUSB.println(u, DEC);
  }
  SerialUSB.println(" -----------------REVERSE!----------------");

  for (int i = 656; i > 0; i--) {
    r = lookup[i];
    SerialUSB.print(r, DEC);
    SerialUSB.print(" , ");
    delay(100);
    SerialUSB.println(u, DEC);
  }
  SerialUSB.println(" -----------------DONE!----------------");
  disableTCInterrupts();
}





void parameterEditmain() {

  SerialUSB.println();
  SerialUSB.println("Edit parameters:");
  SerialUSB.println();
  SerialUSB.println("p ----- position loop");
  SerialUSB.println("v ----- velocity loop");
  SerialUSB.println("o ----- other");
  SerialUSB.println("q ----- quit");
  SerialUSB.println();

  while (SerialUSB.available() == 0)  {}
  char inChar2 = (char)SerialUSB.read();

  switch (inChar2) {
    case 'p':
      {
        parameterEditp();
      }
      break;

    case 'v':
      {
        parameterEditv();
      }
      break;

    case 'o':
      {
        parameterEdito();
      }
      break;
    default:
      {}
      break;



  }
}

void parameterEditp() {
  
  bool quit = false;
  while(!quit){
    SerialUSB.println("Edit position loop gains:");
    SerialUSB.println();
    SerialUSB.print("p ----- pKp = ");
    SerialUSB.println(pKp, DEC);
    SerialUSB.print("i ----- pKi = ");
    SerialUSB.println(pKi, DEC);
    SerialUSB.print("d ----- pKd = ");
    SerialUSB.println(pKd, DEC);
    SerialUSB.print("l----- LPF = ");
    SerialUSB.println(pLPF,DEC);
    SerialUSB.println("q ----- quit");
    SerialUSB.println();
    
    while (SerialUSB.available() == 0)  {}
    char inChar3 = (char)SerialUSB.read();
    
    switch (inChar3) {
      case 'p':
        {
          SerialUSB.println("pKp = ?");
          while (SerialUSB.available() == 0)  {}
          pKp = SerialUSB.parseFloat();
          SerialUSB.print("new pKp = ");
          SerialUSB.println(pKp, DEC);
          SerialUSB.println("");
        }
        break;
      case 'i':
        {
          SerialUSB.println("pKi = ?");
          while (SerialUSB.available() == 0)  {}
          pKi = SerialUSB.parseFloat();
          SerialUSB.print("new pKi = ");
          SerialUSB.println(pKi, DEC);
          SerialUSB.println("");
        }
        break;
      case 'd':
        {
          SerialUSB.println("pKd = ?");
          while (SerialUSB.available() == 0)  {}
          pKd = SerialUSB.parseFloat();
          SerialUSB.print("new pKd = ");
          SerialUSB.println(pKd, DEC);
          SerialUSB.println("");
        }
        break;
       case 'l':
        {
          SerialUSB.println("pLPF = ?");
          while (SerialUSB.available() == 0)  {}
          pLPF = SerialUSB.parseFloat();
          pLPFa = exp(pLPF*-2*3.14159/Fs);
          pLPFb = (1.0-pLPFa);
          SerialUSB.print("new pLPF = ");
          SerialUSB.println(pLPF, DEC);
          SerialUSB.println("");
        }
        break;
      case 'q':
        {  
          quit = true;
          SerialUSB.println("");
          SerialUSB.println("done...");
          SerialUSB.println("");
        }
      default:
        {}
        break;
    }
  }
}

void parameterEditv() {
  bool quit = false;
  while(!quit){  
    SerialUSB.println("Edit velocity loop gains:");
    SerialUSB.println();
    SerialUSB.print("p ----- vKp = ");
    SerialUSB.println(vKp, DEC);
    SerialUSB.print("i ----- vKi = ");
    SerialUSB.println(vKi, DEC);
    SerialUSB.print("d ----- vKd = ");
    SerialUSB.println(vKd, DEC);
    SerialUSB.print("l ----- vLPF = ");
    SerialUSB.println(vLPF, DEC);
    SerialUSB.println("q ----- quit");
    SerialUSB.println();
  
    while (SerialUSB.available() == 0)  {}
    char inChar4 = (char)SerialUSB.read();
  
    switch (inChar4) {
      case 'p':
        {
          SerialUSB.println("vKp = ?");
          while (SerialUSB.available() == 0)  {}
          vKp = SerialUSB.parseFloat();
          SerialUSB.print("new vKp = ");
          SerialUSB.println(vKp, DEC);
        }
        break;
      case 'i':
        {
          SerialUSB.println("vKi = ?");
          while (SerialUSB.available() == 0)  {}
          vKi = SerialUSB.parseFloat();
          SerialUSB.print("new vKi = ");
          SerialUSB.println(vKi, DEC);
        }
        break;
      case 'd':
        {
          SerialUSB.println("vKd = ?");
          while (SerialUSB.available() == 0)  {}
          vKd = SerialUSB.parseFloat();
          SerialUSB.print("new vKd = ");
          SerialUSB.println(vKd, DEC);
        }
        break;
       case 'l':
        {
          SerialUSB.println("vLPF = ?");
          while (SerialUSB.available() == 0)  {}
          vLPF = SerialUSB.parseFloat();
          vLPFa = (exp(vLPF*-2*3.14159/Fs));
          vLPFb = (1.0-vLPFa)* Fs * 0.16666667;
          SerialUSB.print("new vLPF = ");
          SerialUSB.println(vLPF, DEC);
          SerialUSB.println("");
        }
        break;
      case 'q':
        {  
          quit = true;
          SerialUSB.println("");
          SerialUSB.println("done...");
          SerialUSB.println("");  
        }
      default:
        {}
        break;
    }
  }  
}

void parameterEdito() {


  SerialUSB.println("Edit other parameters:");
  SerialUSB.println();
  SerialUSB.print("p ----- PA = ");
  SerialUSB.println(PA, DEC);
  SerialUSB.println();


  while (SerialUSB.available() == 0)  {}
  char inChar3 = (char)SerialUSB.read();

  switch (inChar3) {
    case 'p':
      {
        SerialUSB.println("PA = ?");
        while (SerialUSB.available() == 0)  {}
        PA = SerialUSB.parseFloat();
        SerialUSB.print("new PA = ");
        SerialUSB.println(PA, DEC);
      }

      break;
    default:
      {}
      break;
  }
}



void hybridControl() {        //still under development

  static int missed_steps = 0;
  static float iLevel = 0.6;  //hybrid stepping current level.  In this mode, this current is continuous (unlike closed loop mode). Be very careful raising this value as you risk overheating the A4954 driver!
  static float rSense = 0.15;

  if (yw < r - aps) {
    missed_steps -= 1;
  }
  else if (yw > r + aps) {
    missed_steps += 1;
  }

  output(0.1125 * (-(r - missed_steps)), (255 / 3.3) * (iLevel * 10 * rSense));

}

void serialMenu() {
  SerialUSB.println("");
  SerialUSB.println("");
  SerialUSB.println("----- Mechaduino 0.X -----");
  SerialUSB.print("Firmware: ");
  SerialUSB.println(firmware_version);
  SerialUSB.print("Identifier: ");
  SerialUSB.println(identifier);
  SerialUSB.println("");
  SerialUSB.println("Main menu");
  SerialUSB.println("");
  SerialUSB.println(" s  -  step");
  SerialUSB.println(" d  -  dir");
  SerialUSB.println(" p  -  print angle");
  SerialUSB.println("");
  SerialUSB.println(" c  -  write new calibration table");
  SerialUSB.println(" e  -  check encoder diagnositics");
  SerialUSB.println(" q  -  parameter query");
  SerialUSB.println("");
  SerialUSB.println(" x  -  position mode");
  SerialUSB.println(" v  -  velocity mode");
  SerialUSB.println(" t  -  torque mode");
  SerialUSB.println("");
  SerialUSB.println(" y  -  enable control loop");
  SerialUSB.println(" n  -  disable control loop");
  SerialUSB.println(" r  -  enter new setpoint");
  SerialUSB.println("");
   SerialUSB.println(" j  -  step response");
  SerialUSB.println(" k  -  edit controller gains -- note, these edits are stored in volatile memory and will be reset if power is cycled");
  SerialUSB.println(" g  -  generate sine commutation table");
  SerialUSB.println(" m  -  print main menu");
  // SerialUSB.println(" f  -  get max loop frequency");
  SerialUSB.println("");
}
void sineGen() {
  int temp;
     SerialUSB.println("");
     SerialUSB.println("The sineGen() function in Utils.cpp generates a sinusoidal commutation table.");
     SerialUSB.println("You can experiment with different commutation profiles by modifying this table.");
     SerialUSB.println("The below table should be copied into sine_1 in Parameters.cpp.");   
     SerialUSB.println("");
     delay(3000);
     SerialUSB.println("Printing sine look up table:...");
     SerialUSB.println("");
  for (int x = 0; x <= 3600; x++) {
    //temp = round(1024.0 * sin((3.14159265358979 * ((x * 0.1 / 180.0) + 0.25))));
    temp = round(1024.0 * sin((3.14159265358979 * ((x * 0.1 / 180.0) + 0.0))));
   SerialUSB.print(temp);
   SerialUSB.print(", ");  
  }

}



void stepResponse() {     // not done yet...
  SerialUSB.println("");
  SerialUSB.println("--------------------------------");
  SerialUSB.println("");
  SerialUSB.println("Get ready for step response!");
  SerialUSB.println("Close Serial Monitor and open Tools>>Serial Plotter");
  SerialUSB.println("You have 10 seconds...");
  enableTCInterrupts();     //start in closed loop mode
  //mode = 'x';
  r = 0;
  delay(1000);
  SerialUSB.println("9...");
  delay(1000);
  SerialUSB.println("8...");
  delay(1000);
  SerialUSB.println("7...");
  delay(1000);
  SerialUSB.println("6...");
  delay(1000);
  SerialUSB.println("5...");
  delay(1000);
  SerialUSB.println("4...");
  delay(1000);
  SerialUSB.println("3...");
  delay(1000);
  SerialUSB.println("2...");
  delay(1000);
  SerialUSB.println("1...");
  delay(1000);
  print_yw = true;
  delay(100);
  r = 97.65;      /// choose step size as you like, 97.65 gives a nice plot since 97.65*1024 = 10,000
  delay(400);
  print_yw = false;
  r = 0;
  delay(500);
  disableTCInterrupts();

}



void moveRel(float pos_final,int vel_max, int accel){
  
   //Use this function for slow relative movements in closed loop position mode
   //
   // This function creates a "trapezoidal speed" trajectory (constant accel, and max speed, constant decel);
   // It works pretty well, but it may not be perfect
   // 
   // pos_final is the desired position in degrees
   // vel_max is the max velocity in degrees/second
   // accel is the max accel in degrees/second^2
   //
   //Note that the actual max velocity is limited by the execution speed of all the math below.
   //Adjusting dpos (delta position, or step size) allows you to trade higher speeds for smoother motion
   //Max speed with dpos = 0.225 degrees is about 180 deg/sec
   //Max speed with dpos = 0.45 degrees is about 360 deg/sec 
  
  float pos = 0;
  float dpos = 0.45;  // "step size" in degrees, smaller is smoother, but will limit max speed, keep below stepper step angle
  float vel = 0;      // 
  float vel_1 =0;
  int start = micros(); //for debugging

  float accel_x_dpos = accel*dpos;  // pre calculate
  float dpos_x_1000000 = dpos*1000000.0; // pre calculate

  float pos_remaining = pos_final-pos;
  unsigned long dt =0; 
  unsigned long t = micros();
  unsigned long t_1 = t;

  float r0 = r;  //hold initial setpoint

  // Assume we're decelerating and calculate speed along deceleration profile
  
  while (abs(pos_remaining) >(dpos/2)){  //(may not actually reach exactly so leave some margin
  
    if (pos_remaining > 0)        // clockwise
    vel = sqrt(2.0 * pos_remaining * accel);
    else                      // counter clockwise
    vel = -sqrt(2.0 * -pos_remaining * accel);

    if (vel > vel_1)  // Check if we actually need to accelerate in  clockwise direction
      {

      if (vel_1 == 0)  
        vel = sqrt(2.0 * accel_x_dpos);
      else
        vel = vel_1 + abs(accel_x_dpos/ vel_1);
      if (vel > vel_max)
        vel = vel_max;
      }
    else if (vel < vel_1)
    {
    // Need to accelerate in  counter clockwise direction
    if (vel_1 == 0)
      vel = -sqrt(2.0 * accel_x_dpos);
    else
      vel = vel_1 - abs(accel_x_dpos/ vel_1);
    if (vel < -vel_max)
      vel = -vel_max;
    }
  //  SerialUSB.println(vel);
  
 
  dt = abs(dpos_x_1000000 / vel);
  
    while(t < t_1 + dt) {           //wait calculated dt 
    t = micros();
    }
  
  if (vel > 0)  pos += dpos;        //update setpoint
  else if (vel < 0) pos -= dpos;
  r= r0 + pos;
  
  //SerialUSB.print(micros()-start);
  //SerialUSB.print(" , ");
  
  t_1 = t;  
  vel_1 = vel;
  pos_remaining = pos_final-pos;
  
  }
  r = r0 +pos_final;
  //SerialUSB.print(micros()-start);
  
}




void moveAbs(float pos_final,int vel_max, int accel){
  
   //Use this function for slow absolute movements in closed loop position mode
   //
   // This function creates a "trapezoidal speed" trajectory (constant accel, and max speed, constant decel);
   // It works pretty well, but it may not be perfect
   // 
   // pos_final is the desired position in degrees
   // vel_max is the max velocity in degrees/second
   // accel is the max accel in degrees/second^2
   //
   //Note that the actual max velocity is limited by the execution speed of all the math below.
   //Adjusting dpos (delta position, or step size) allows you to trade higher speeds for smoother motion
   //Max speed with dpos = 0.225 degrees is about 180 deg/sec
   //Max speed with dpos = 0.45 degrees is about 360 deg/sec
  
  float pos = r;
  float dpos = 0.225;  // "step size" in degrees, smaller is smoother, but will limit max speed, keep below stepper step angle
  float vel = 0;      // 
  float vel_1 =0;
 // int start = micros(); //for debugging

  float accel_x_dpos = accel*dpos;  // pre calculate
  float dpos_x_1000000 = dpos*1000000.0; // pre calculate

  float pos_remaining = pos_final-pos;
  unsigned long dt =0; 
  unsigned long t = micros();
  unsigned long t_1 = t;


  // Assume we're decelerating and calculate speed along deceleration profile
  
  while (abs(pos_remaining) >(dpos/2)){  //(may not actually reach exactly so leave some margin
  
    if (pos_remaining > 0)        // clockwise
    vel = sqrt(2.0 * pos_remaining * accel);
    else                      // counter clockwise
    vel = -sqrt(2.0 * -pos_remaining * accel);

    if (vel > vel_1)  // Check if we actually need to accelerate in  clockwise direction
      {

      if (vel_1 == 0)  
        vel = sqrt(2.0 * accel_x_dpos);
      else
        vel = vel_1 + abs(accel_x_dpos/ vel_1);
      if (vel > vel_max)
        vel = vel_max;
      }
    else if (vel < vel_1)
    {
    // Need to accelerate in  counter clockwise direction
    if (vel_1 == 0)
      vel = -sqrt(2.0 * accel_x_dpos);
    else
      vel = vel_1 - abs(accel_x_dpos/ vel_1);
    if (vel < -vel_max)
      vel = -vel_max;
    }
  //  SerialUSB.println(vel);
  
 
  dt = abs(dpos_x_1000000 / vel);
  
    while(t < t_1 + dt) {           //wait calculated dt 
    t = micros();
    }
  
  if (vel > 0)  pos += dpos;        //update setpoint
  else if (vel < 0) pos -= dpos;
  r= pos;
  
  //SerialUSB.print(micros()-start);    //for debugging
  //SerialUSB.print(" , ");
  
  t_1 = t;  
  vel_1 = vel;
  pos_remaining = pos_final-pos;
  
  }
  r = pos_final;
  //SerialUSB.print(micros()-start);
  
}
