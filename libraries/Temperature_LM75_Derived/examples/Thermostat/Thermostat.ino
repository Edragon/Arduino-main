/*
The LM75 provides a "thermostat"-like behavior primarily to allow control
of cooling fans based on temperature thresholds using an ALERT pin (typically
pin 3). There are two registers provided, which behave the following way in
"comparator" mode (the default):

  T_high (or T_os):
    The threshold at which the ALERT output should be enabled as the sensor
    temperature increases.

  T_low (or T_hyst):
    The threshold at which the ALERT output should be disabled as the sensor
    temperature decreases. This ensures that there can be hysteresis so that
    the output is not rapidly enabled and disabled.

The ALERT output pin can usually be active-high or active-low depending on
the configuration register's polarity bit (see setAlertPolarity()).

Additionally, an "interrupt" mode is available which can be used to trigger
an interrupt upon the sensor crossing a the T_high -or- T_low thresholds
(see setThermostatInterruptMode()).
*/

#include <Temperature_LM75_Derived.h>

// Which Arduino pin has the sensor's ALERT output been connected to?
#define SENSOR_ALERT_PIN 4

// Should the internal pullup be enabled on the ALERT pin? If not, comment out.
#define SENSOR_ALERT_PULLUP

// The Generic_LM75 class provides for 9-bit (±0.5°C) resolution. For improved
// resolution, use one of the sensor-specific classes.
Generic_LM75 temperature;

void setup() {
  while(!Serial) {}

  Serial.begin(9600);

#ifdef SENSOR_ALERT_PULLUP
  // Enable the internal pullup
  pinMode(SENSOR_ALERT_PIN, OUTPUT);
  digitalWrite(SENSOR_ALERT_PIN, HIGH);
#endif

  Wire.begin();

  // Enable the ALERT pin as active-high.
  temperature.setAlertActiveHigh();

  // At what temperature should ALERT be enabled when the temperature is
  // increasing? Chosen so that touching the sensor can raise the temperature
  // enough to enable ALERT.
  temperature.setTemperatureHighC(27.0);

  // At what temperature should ALERT be disabled when the temperature is
  // decreasing (to provided hysteresis and prevent rapid cycling of the
  // ALERT pin)?
  temperature.setTemperatureLowC(25.0);
}

void loop() {
  Serial.print("T = ");
  Serial.print(temperature.readTemperatureC());

  Serial.print(", T_low = ");
  Serial.print(temperature.readTemperatureLowC());

  Serial.print(", T_high = ");
  Serial.print(temperature.readTemperatureHighC());

  Serial.print(", alert (pin ");
  Serial.print(SENSOR_ALERT_PIN);
  Serial.print(") = ");
  Serial.print(digitalRead(SENSOR_ALERT_PIN) ? "HIGH" : "LOW");

  Serial.println();

  delay(250);
}
