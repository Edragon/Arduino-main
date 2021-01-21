#include "Ai_AP3216_AmbientLightAndProximity.h"

// please check https://www.roboremo.com/reading-ap3216-with-arduino.html
// this example is adopted code from there (parts regarding sensor are moved to Library)

//**************** COMMAND
char cmd[100];
int cmdIndex;
void exeCmd();

bool dbg_on = true;
Ai_AP3216_AmbientLightAndProximity aps = Ai_AP3216_AmbientLightAndProximity();
//Ai_AP3216_AmbientLightAndProximity aps = Ai_AP3216_AmbientLightAndProximity(D5, D6);//custompins

void setup() {
	Serial.begin(115200);
	uint8_t resolutionCoef_1_to_16 = 16;
	uint8_t resolution_0_to_3 = 3;
	uint8_t interruptFilter_0_to_3 = 0;
	uint8_t numPulses_0_to_3 = 3;
	uint8_t ledCurrent_0_to_3 = 3;

	aps.setProximitySensorParams(
		resolutionCoef_1_to_16, resolution_0_to_3,
		interruptFilter_0_to_3, numPulses_0_to_3, ledCurrent_0_to_3
	);
	aps.begin();
	aps.startAmbientLightAndProximitySensor ();
}

void loop() {

	while (Serial.available()) {
		char c = Serial.read();

		if (c == '\n' || c == '\r') {
			cmd[cmdIndex] = 0; // end cmd string with 0
			exeCmd();  // execute the command
			cmdIndex = 0; // reset the cmdIndex
		}
		else {
			cmd[cmdIndex] = c; // append c to the cmd string
			if (cmdIndex<99) cmdIndex++;
		}
	}

	long alsValue = aps.getAmbientLight();
	long psValue = aps.getProximity();

	if (dbg_on) {
		if (aps.isAmbientLightSensorOn()) { Serial.print("A: ");    Serial.print(alsValue); };
		if (aps.isProximityLightSensorOn()) { Serial.print(" P: ");    Serial.print(psValue); };
		if (aps.isAmbientLightSensorOn() || aps.isProximityLightSensorOn()) { Serial.print("\n"); };
	}

	if (aps.isAmbientLightSensorOn() && aps.isProximityLightSensorOn()) {
		bool isClose = false;
		
		//for normal light intensity
		if (alsValue < 1000 && psValue>300) isClose = true; 

		//when light intensity is low, proximity is not so sensitive
		if (alsValue < 500 && psValue>200) isClose = true; 

		if (psValue>800) isClose = true; //independent on light when proximity is very high value...
		
		if (isClose) {
			Serial.print(millis()/1000);
			Serial.print(" \n");
		}
	}

	delay(200);
	

}



//**************** COMMAND (copied from https://www.roboremo.com/reading-ap3216-with-arduino.html )

boolean cmdStartsWith(const char *st) { // checks if cmd starts with st
	for (int i = 0; ; i++) {
		if (st[i] == 0) return true;
		if (cmd[i] == 0) return false;
		if (cmd[i] != st[i]) return false;;
	}
	return false;
}

int hexCharToInt(char c) {
	if (c >= 'a') return (c - 'a') + 10;
	if (c >= 'A') return (c - 'A') + 10;
	return c - '0';
}

String hexByteToString(int val) {
	char digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8','9', 'A', 'B', 'C', 'D', 'E', 'F' };

	char a = digits[(val / 16) % 16];
	char b = digits[val % 16];

	return (String) "" + a + b;
}

void exeCmd() {

	if (cmdStartsWith("read 0x")) {  // example: read 0x1A
		int a = hexCharToInt(cmd[7]); // '1' -> 1
		int b = hexCharToInt(cmd[8]); // 'A' -> 10

		int regAddress = (a * 16) + b; // 0x1A = 26

		int regValue = aps.AP3216_read(regAddress);

		Serial.print((String)"reg_0x");
		Serial.print(hexByteToString(regAddress));
		Serial.print(" = ");
		Serial.print(hexByteToString(regValue));
		Serial.print("  "); 
		Serial.print(regValue,BIN );
		Serial.print("\n");
	}


	if (cmdStartsWith("write 0x")) {  // example: write 0x1A 0x55
		int a = hexCharToInt(cmd[8]); // '1' -> 1
		int b = hexCharToInt(cmd[9]); // 'A' -> 10
		int regAddress = (a * 16) + b; // 0x1A = 26

		a = hexCharToInt(cmd[13]);
		b = hexCharToInt(cmd[14]);
		int regValue = (a * 16) + b;

		aps.AP3216_write(regAddress, regValue);

		Serial.print((String)"reg_0x");
		Serial.print(hexByteToString(regAddress));
		Serial.print(" <- ");
		Serial.print(hexByteToString(regValue));
		Serial.print("\n");
	}

	if (cmdStartsWith("a start")) {
		aps.startOnlyAmbientLightSensor();
	}

	if (cmdStartsWith("all start")) {
		aps.startAmbientLightAndProximitySensor();
	}

	if (cmdStartsWith("debug on")) {
		dbg_on = true;
	}
	if (cmdStartsWith("debug off")) {
		dbg_on = false;
	}

	if (cmdStartsWith("stop")) {
		aps.stopAmbientLightAndProximitySensor();
	}

	if (cmdStartsWith("p start")) {
		aps.startOnlyProximitySensor();
	}
}
