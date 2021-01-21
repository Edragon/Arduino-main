// 
// 
// 

#include "Ai_AP3216_AmbientLightAndProximity.h"



uint8_t AP3216_iic_address ;// I2C Address of AP3216 sensor is 0x1E
int pin_sda = -1;
int pin_scl = -1;

//............ constructors
Ai_AP3216_AmbientLightAndProximity::Ai_AP3216_AmbientLightAndProximity() {
	AP3216_iic_address = AI_AP3216_IIC_ADDRESS;
	pin_sda = -1;
	pin_scl = -1;
}

Ai_AP3216_AmbientLightAndProximity::Ai_AP3216_AmbientLightAndProximity(int sda, int scl) {
	AP3216_iic_address = AI_AP3216_IIC_ADDRESS;
	pin_sda = sda;
	pin_scl = scl;
}

Ai_AP3216_AmbientLightAndProximity::Ai_AP3216_AmbientLightAndProximity(uint8_t iic_address, int sda, int scl) {
	AP3216_iic_address = iic_address;
	pin_sda = sda;
	pin_scl = scl;
}

//............ begin
void Ai_AP3216_AmbientLightAndProximity::begin() {
#if defined(ESP8266) || defined(ESP32) 
	if (pin_sda >= 0 && pin_scl >= 0) {
		Wire.begin(pin_sda, pin_scl);
	} else {
		Wire.begin();
	}
#else
	Wire.begin();
#endif

	beginWithoutOpeningWire();
}

void Ai_AP3216_AmbientLightAndProximity::beginWithoutOpeningWire() {
	AP3216_write(0x00, 0x03); //by default turn on both ALS and PS
}

//..............start
void Ai_AP3216_AmbientLightAndProximity::stopAmbientLightAndProximitySensor() {
	als_on = false;
	ps_on = false;
	AP3216_write(0x00, 0x00);
}


void Ai_AP3216_AmbientLightAndProximity::startOnlyAmbientLightSensor() {
	AP3216_write(0x00, 0x01);
	als_on = true;
}

void Ai_AP3216_AmbientLightAndProximity::startOnlyProximitySensor() {
	AP3216_write(0x00, 0x02);
	als_on = false;
	ps_on = true;
}

void Ai_AP3216_AmbientLightAndProximity::startAmbientLightAndProximitySensor() {
	AP3216_write(0x00, 0x03);
	als_on = true;
	ps_on = true;
}


//.............. set

void Ai_AP3216_AmbientLightAndProximity::setProximitySensorParams(
	uint8_t resolutionCoef_1_to_16, uint8_t resolution_0_to_3, uint8_t interruptFilter_0_to_3,
	uint8_t numPulses_0_to_3, uint8_t ledCurrent_0_to_3
) {
	//PS config 0x20
	if (resolutionCoef_1_to_16 > 0) {
		//convert to zero based
		resolutionCoef_1_to_16 = resolutionCoef_1_to_16 - 1;
		resolutionCoef_1_to_16 &= 0x0F;
	}

	resolution_0_to_3 &= 0x03;
	interruptFilter_0_to_3 &= 0x03;
	int value = (resolutionCoef_1_to_16 << 4) + (resolution_0_to_3 << 2) + interruptFilter_0_to_3;
	AP3216_write(0x20, value); //Default = 0x13


	//PS LED params 0x21
	numPulses_0_to_3 = numPulses_0_to_3 && 0x03;
	ledCurrent_0_to_3 = ledCurrent_0_to_3 && 0x03;
	value = (numPulses_0_to_3 << 8) + ledCurrent_0_to_3;
	AP3216_write(0x21, value); //Default = 0x13
}

//............. get
long Ai_AP3216_AmbientLightAndProximity::getAmbientLight() {
	long alsValue = 65535;
	if (als_on) {
		alsValue = (AP3216_read(0x0D) << 8) + (AP3216_read(0x0C));
	}
	return alsValue;
}

long Ai_AP3216_AmbientLightAndProximity::getProximity() {
	long psValue = 0;
	if (ps_on) {
		int a = AP3216_read(0x0F) & 0b00111111; // PS Data HIGH 6 bits
		int b = AP3216_read(0x0E) & 0b00001111; // PS Data LOW 4 bits
		psValue = (a << 4) + b;
	}
	return psValue;
}


//private
//helpers
void Ai_AP3216_AmbientLightAndProximity::AP3216_write(int regAddress, int value) {
	Wire.beginTransmission(AP3216_iic_address);
	Wire.write(regAddress);
	Wire.write(value);
	Wire.endTransmission();
}

int Ai_AP3216_AmbientLightAndProximity::AP3216_read(int regAddress) {
	Wire.beginTransmission(AP3216_iic_address);
	Wire.write(regAddress);
	Wire.endTransmission();
#if defined(ESP8266) || defined(ESP32) 
	Wire.requestFrom(AP3216_iic_address, (size_t)1, true);
#else
	Wire.requestFrom(AP3216_iic_address, (size_t)1);	
#endif
	return Wire.read() & 0xFF;
}
