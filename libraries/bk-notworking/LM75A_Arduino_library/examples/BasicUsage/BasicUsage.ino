//---------------------------------------------------------------------------------------------
//
// Basic usage example for the M2M_LM75A Arduino library.
//
// Copyright 2016-2017, M2M Solutions AB
// Written by Jonny Bergdahl, 2016-11-18
//
// Licensed under the MIT license, see the LICENSE.txt file.
//
//---------------------------------------------------------------------------------------------
// 2017-04-19 Added begin() and end() functions for Wire handling.
// 2017-04-19 Fixed a code merge problem
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include <M2M_LM75A.h>

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Global variables
//
M2M_LM75A lm75a;

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Setup
//
void setup()
{
  lm75a.begin();
  while (!Serial); // Leonardo: wait for serial monitor  
  Serial.begin(115200);
  Serial.println(F("M2M_LM75A - Basic usage"));
  Serial.println(F("==========================================="));
  Serial.println("");
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Loop
//
void loop()
{
  // Temperature
  Serial.print(F("Temperature in Celsius: "));
  Serial.print(lm75a.getTemperature());
  Serial.println(F(" *C"));

  Serial.print(F("Temperature in Farenheit: "));
  Serial.print(lm75a.getTemperatureInFarenheit());
  Serial.println(F(" *F"));

  Serial.print(F("Hysteris temperature: "));
  Serial.print(lm75a.getHysterisisTemperature());
  Serial.println(F(" *C"));

  Serial.print(F("OS trip temperature: "));
  Serial.print(lm75a.getOSTripTemperature());
  Serial.println(F(" *C"));

  // Shutdown/Wake up
  Serial.println(F("Shutting down"));
  lm75a.shutdown();

  Serial.print(F("Is shutdown: "));
  Serial.println(lm75a.isShutdown());
  delay(5000);

  Serial.println(F("Waking up"));
  lm75a.wakeup();
  Serial.print(F("Is shutdown: "));
  Serial.println(lm75a.isShutdown());
  delay(1000);
  Serial.println(F(""));  
  Serial.println(F("==========================================="));
  Serial.println(F(""));
}