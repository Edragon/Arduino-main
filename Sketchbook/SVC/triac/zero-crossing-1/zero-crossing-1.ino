//source: http://electronics.stackexchange.com/q/59615

int AC_LOAD = 3;    // Output to Opto Triac pin
//int AC_LOAD2 = 4;    // Output to Opto Triac pin
int dimming = 30;  // Dimming level (0-128)  0 = ON, 128 = OFF

void setup()
{
  pinMode(AC_LOAD, OUTPUT);       // Set the AC Load as output
  //attachInterrupt(digitalPinToInterrupt(2), zero_crosss_int, RISING);
  //attachInterrupt(digitalPinToInterrupt(2), zero_crosss_int, CHANGE);
  attachInterrupt(0, zero_crosss_int, RISING);  // Choose the zero cross interrupt # from the table above
}

void zero_crosss_int()  // function to be fired at the zero crossing to dim the light
{
  // Firing angle calculation :: 50Hz-> 10ms (1/2 Cycle)
  // (10000us - 10us) / 128 = 75 (Approx)
  
  int dimtime = (75 * dimming);    // 75 for 50hz china, 65 for 60hz
  delayMicroseconds(dimtime);    // Off cycle
  digitalWrite(AC_LOAD, HIGH);   // triac firing
  //digitalWrite(AC_LOAD2, HIGH);   // triac firing
  
  delayMicroseconds(10);         // triac On propogation delay
  digitalWrite(AC_LOAD, LOW);    // triac Off
  //digitalWrite(AC_LOAD2, LOW);    // triac Off
}

void loop()
{
  dimming = 128;
  delay(1000);
  dimming = 75;
  delay(1000);
  dimming = 25;
  delay(1000);
  dimming = 0;
  delay(1000);
  dimming = 25;
  delay(1000);
  dimming = 75;
  delay(1000);
  dimming = 128;
  delay(1000);
}
