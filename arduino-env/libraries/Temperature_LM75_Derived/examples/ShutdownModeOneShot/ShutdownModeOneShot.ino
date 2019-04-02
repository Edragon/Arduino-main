/*
Shut down the sensor, and use one-shot conversion to request a single immediate
temperature conversion.

In order to save power, LM75-derived temperature sensors can be placed in
"shutdown" mode where they stop making regular temperature conversions. Some
sensors support a "one-shot" mode to request a single temperature conversion
from an otherwise shutdown sensor. This can be used for two purposes:

  * To conserve (a lot of) power by making very infrequent temperature
    conversions, such as every few minutes.

  * To convert more frequently than the usually-supported temperature
    conversion rates allow on very fast sensors (such as TMP102/TMP112).
*/

#include <Temperature_LM75_Derived.h>

Generic_LM75_9_to_12Bit_OneShot temperature;

void setup() {
  while(!Serial) {}
  
  Serial.begin(9600);

  Wire.begin();

  temperature.enableShutdownMode();
}

void loop() {
  Serial.println("Starting temperature conversion...");
  temperature.startOneShotConversion();

  Serial.println("Waiting for conversion to be ready...");
  int waited_ms = 0;
  while(!temperature.checkConversionReady()) {
    waited_ms++;
    delay(1);
  }
  
  Serial.print("Conversion ready in ");
  Serial.print(waited_ms);
  Serial.print(" ms; temperature is: ");
  Serial.print(temperature.readTemperatureC());
  Serial.println(" C!");
  Serial.println();

  delay(250);
}
