#include "Ai_AP3216_AmbientLightAndProximity.h"

Ai_AP3216_AmbientLightAndProximity aps = Ai_AP3216_AmbientLightAndProximity();
//Ai_AP3216_AmbientLightAndProximity aps = Ai_AP3216_AmbientLightAndProximity(D5, D6);//custompins

void setup() {
	Serial.begin(115200);
	aps.begin();
	aps.startAmbientLightAndProximitySensor ();
}

void loop() {
	long alsValue = aps.getAmbientLight();
	long psValue = aps.getProximity();

	Serial.print("Ambient Light: ");
	Serial.print(alsValue);
	Serial.print(", Proximity: ");
	Serial.println(psValue);

	delay(200);
}

