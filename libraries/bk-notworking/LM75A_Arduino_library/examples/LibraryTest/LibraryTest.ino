//---------------------------------------------------------------------------------------------
//
// Full library test example for the M2M_LM75A Arduino library.
//
// Copyright 2016-2017, M2M Solutions AB
// Written by Jonny Bergdahl, 2016-11-18
//
// Licensed under the MIT license, see the LICENSE.txt file.
//
//---------------------------------------------------------------------------------------------
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
bool testResult = true;
float oldHysterisisTemperature;
float oldOsTripTemperature;
FaultQueueValue oldFaultQueueValue;
OsPolarity oldOsPolarity;
DeviceMode oldDeviceMode;
float newHysterisisTemperature = 62.0f;
float newOsTripTemperature = 59.0f;
FaultQueueValue newFaultQueueValue = FaultQueueValue::NUMBER_OF_FAULTS_6;
OsPolarity newOsPolarity = OsPolarity::OS_POLARITY_ACTIVEHIGH;
DeviceMode newDeviceMode = DeviceMode::DEVICE_MODE_INTERRUPT;

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Setup
//
void setup()
{
  lm75a.begin();
  while (!Serial); // Leonardo: wait for serial monitor
  Serial.begin(115200);
  Serial.println("Running M2M_LM75A Arduino library full test");
  Serial.println("===========================================");
  Serial.println("");
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Loop
//
void loop()
{
  float temperature;

  Serial.println("Testing communication");
  checkTrueValue("isConnected", lm75a.isConnected(), true);
  Serial.println();

  Serial.println("Testing temperature getters");
  Serial.println("---------------------------");
  checkTemperatureResult("getTemperature", lm75a.getTemperature());
  checkTemperatureResult("getTemperatureInFarenheit", lm75a.getTemperatureInFarenheit());
  checkTemperatureResult("getHysterisisTemperature", lm75a.getHysterisisTemperature());
  checkTemperatureResult("getOSTripTemperature", lm75a.getOSTripTemperature());
  Serial.println();

  Serial.println("Testing other getters");
  Serial.println("---------------------");  
  checkFaultQueueValue("getFaultQueueValue", lm75a.getFaultQueueValue(), FaultQueueValue::NUMBER_OF_FAULTS_1);
  checkOsPolarityValue("getOsPolarity", lm75a.getOsPolarity(), OsPolarity::OS_POLARITY_ACTIVELOW);
  checkDeviceModeValue("getDeviceMode", lm75a.getDeviceMode(), DeviceMode::DEVICE_MODE_COMPARATOR);
  Serial.println();

  Serial.println("Testing temperature setters");
  Serial.println("---------------------------"); 
  oldHysterisisTemperature = lm75a.getHysterisisTemperature();
  oldOsTripTemperature = lm75a.getOSTripTemperature();

  lm75a.setHysterisisTemperature(newHysterisisTemperature);
  lm75a.setOsTripTemperature(newOsTripTemperature);

  checkTemperatureValue("setHysterisisTemperature", lm75a.getHysterisisTemperature(), newHysterisisTemperature);
  checkTemperatureValue("setOsTripTemperature", lm75a.getOSTripTemperature(), newOsTripTemperature);

  lm75a.setHysterisisTemperature(oldHysterisisTemperature);
  lm75a.setOsTripTemperature(oldOsTripTemperature);
  Serial.println();

  Serial.println("Testing other setters");
  Serial.println("---------------------");      
  oldFaultQueueValue = lm75a.getFaultQueueValue();
  oldOsPolarity = lm75a.getOsPolarity();
  oldDeviceMode = lm75a.getDeviceMode();

  lm75a.setFaultQueueValue(newFaultQueueValue);
  lm75a.setOsPolarity(newOsPolarity);
  lm75a.setDeviceMode(newDeviceMode);
  checkFaultQueueValue("setFaultQueueValue: ", lm75a.getFaultQueueValue(), newFaultQueueValue);
  checkOsPolarityValue("setOsPolarity", lm75a.getOsPolarity(), newOsPolarity);
  checkDeviceModeValue("setDeviceMode", lm75a.getDeviceMode(), newDeviceMode);

  lm75a.setFaultQueueValue(oldFaultQueueValue);
  lm75a.setOsPolarity(oldOsPolarity);
  lm75a.setDeviceMode(oldDeviceMode);

  Serial.println();

  Serial.println("Testing shutdown");
  Serial.println("----------------");      
  lm75a.shutdown();
  checkTrueValue("shutdown", lm75a.isShutdown(), true);
  lm75a.wakeup();
  checkTrueValue("shutdown", lm75a.isShutdown(), false);
  Serial.println();

  Serial.print("==== ");
  Serial.print("Test result: ");
  Serial.print(testResult ? "OK" : "FAILED");
  Serial.print(" ====");

  while (true)
    ;
}

void checkTemperatureResult(const char* caption, const float value)
{
  bool fail = value == LM75A_INVALID_TEMPERATURE;
  if (fail)
  {
    testResult = false;
  }
  Serial.print(caption);
  Serial.print(": ");
  if (fail)
  {
    Serial.print("LM75A_INVALID_TEMPERATURE");
  }
  else
  {
    Serial.print(value);
  }
  Serial.print(" - ");
  Serial.println(fail ? "FAIL" : "OK");
}

void checkTemperatureValue(const char* caption, const float value, const float expected)
{
  bool fail = value != expected;
  Serial.print(caption);
  Serial.print(": ");
  Serial.print(value);
  Serial.print(" - ");
  if (fail)
  {
    testResult = false;
    Serial.print("FAIL (");
    Serial.print(expected);
    Serial.println(")");
  }
  else
  {
    Serial.println("OK");
  }
}

void check8BitValue(const char* caption, const uint8_t value, const uint8_t expected)
{
  bool fail = value != expected;
  Serial.print(caption);
  Serial.print(": ");
  Serial.print(value);
  Serial.print(" - ");
  if (fail)
  {
    testResult = false;
    Serial.print("FAIL (");
    Serial.print(expected);
    Serial.println(")");
  }
  else
  {
    Serial.println("OK");
  }
}

void checkFaultQueueValue(const char* caption, const FaultQueueValue value, const FaultQueueValue expected)
{
  bool fail = value != expected;
  Serial.print(caption);
  Serial.print(": ");
  Serial.print(getFaultQueueValueString(value));
  Serial.print(" - ");
  if (fail)
  {
    testResult = false;
    Serial.print("FAIL (");
    Serial.print(getFaultQueueValueString(expected));
    Serial.println(")");
  }
  else
  {
    Serial.println("OK");
  }  
}

void checkOsPolarityValue(const char* caption, const OsPolarity value, const OsPolarity expected)
{
  bool fail = value != expected;
  Serial.print(caption);
  Serial.print(": ");
  Serial.print(getOsPolarityString(value));
  Serial.print(" - ");
  if (fail)
  {
    testResult = false;
    Serial.print("FAIL (");
    Serial.print(getOsPolarityString(expected));
    Serial.println(")");
  }
  else
  {
    Serial.println("OK");
  }
}

void checkDeviceModeValue(const char* caption, const DeviceMode value, const DeviceMode expected)
{
  bool fail = value != expected;
  Serial.print(caption);
  Serial.print(": ");
  Serial.print(getDeviceModeString(value));
  Serial.print(" - ");
  if (fail)
  {
    testResult = false;
    Serial.print("FAIL (");
    Serial.print(getDeviceModeString(expected));
    Serial.println(")");
  }
  else
  {
    Serial.println("OK");
  }
}

void checkTrueValue(const char* caption, const bool value, const bool expected)
{
  bool fail = value != expected;
  Serial.print(caption);
  Serial.print(": ");
  Serial.print(value ? "True" : "False");
  Serial.print(" - ");
  if (fail)
  {
    testResult = false;
    Serial.print("FAIL (");
    Serial.print(expected);
    Serial.println(")");
  }
  else
  {
    Serial.println("OK");
  }
}

const char* getFaultQueueValueString(const FaultQueueValue value)
{
  switch (value)
  {
    case FaultQueueValue::NUMBER_OF_FAULTS_1:
      return "NUMBER_OF_FAULTS_1";
    case FaultQueueValue::NUMBER_OF_FAULTS_2:
      return "NUMBER_OF_FAULTS_2";
    case FaultQueueValue::NUMBER_OF_FAULTS_4:
      return "NUMBER_OF_FAULTS_4";
    case FaultQueueValue::NUMBER_OF_FAULTS_6:
      return "NUMBER_OF_FAULTS_4";
    default:
      return "** ILLEGAL VALUE **";
  }
}

const char* getOsPolarityString(const OsPolarity value)
{
  switch (value)
  {
    case OsPolarity::OS_POLARITY_ACTIVEHIGH:
      return "OS_POLARITY_ACTIVEHIGH";
    case OsPolarity::OS_POLARITY_ACTIVELOW:
      return "OS_POLARITY_ACTIVELOW";
    default:
      return "** ILLEGAL VALUE **";
  }
}

const char* getDeviceModeString(DeviceMode value)
{
  switch (value)
  {
    case DeviceMode::DEVICE_MODE_COMPARATOR:
      return "DEVICE_MODE_COMPARATOR";
    case DeviceMode::DEVICE_MODE_INTERRUPT:
      return "DEVICE_MODE_INTERRUPT";
    default:
      return "** ILLEGAL VALUE **";
  }
}

