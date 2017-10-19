
#include "power.h"
//#include <FS.h> 


ESP8266PowerClass power_dev;
unsigned long previousMillis = 0;
const long interval = 2000;


double power;
double current;
double cfrequency;
double voltage;

void setup() {
  Serial.begin(115200);
  //SPIFFS.format();
  //Serial.println("mounting FS...");
  //Serial.begin(74880);
  //Serial1.begin(115200);
  //pinMode(16, OUTPUT);
  //digitalWrite(16, HIGH);


  power_dev.setPowerParam(12.65801022, 0.0);
  power_dev.setVoltageParam(0.45039823, 0.0);
  power_dev.setCurrentParam(19.52, -85.9);

  power_dev.enableMeasurePower();
  power_dev.selectMeasureCurrentOrVoltage(CURRENT);
  power_dev.startMeasure(); // mills

}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    power = power_dev.getPower();
    current = power_dev.getCurrent();
    voltage = power_dev.getVoltage();

    cfrequency = power_dev.getCurrFrequency();
    Serial.print("power: ");
    Serial.println(power);
    Serial.print("current: ");
    Serial.println(current);
    Serial.print("voltage: ");
    Serial.println(voltage);
    Serial.print("current frequency: ");
    Serial.println(cfrequency);
  }
  // yield();
}

