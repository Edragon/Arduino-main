// Ai_AP3216_AmbientLightAndProximity.h

#ifndef _AI_AP3216_AMBIENTLIGHTANDPROXIMITY_h
#define _AI_AP3216_AMBIENTLIGHTANDPROXIMITY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include <Wire.h>

#define AI_AP3216_IIC_ADDRESS 0x1E

class Ai_AP3216_AmbientLightAndProximity {

private:


	bool als_on = false;
	bool ps_on = false;
	bool dbg_on = false;

public:
	Ai_AP3216_AmbientLightAndProximity();
	Ai_AP3216_AmbientLightAndProximity(int sda, int scl);
	Ai_AP3216_AmbientLightAndProximity(uint8_t iic_address, int sda, int scl);

	void begin();
	void beginWithoutOpeningWire();

	void stopAmbientLightAndProximitySensor();
	void startOnlyAmbientLightSensor();
	void startOnlyProximitySensor(); 
	void startAmbientLightAndProximitySensor();

	void setProximitySensorParams(
		uint8_t resolutionCoef_1_to_16, uint8_t resolution_0_to_3, uint8_t interruptFilter_0_to_3,
		uint8_t numPulses_0_to_3, uint8_t ledCurrent_0_to_3
	);

	long getAmbientLight();
	long getProximity();

	bool isAmbientLightSensorOn() { return als_on; };
	bool isProximityLightSensorOn() { return ps_on; };


	//public for now...
	void AP3216_write(int regAddress, int value);
	int AP3216_read(int regAddress);
};

#endif

