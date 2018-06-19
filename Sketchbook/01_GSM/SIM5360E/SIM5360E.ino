#include <AltSoftSerial.h>

// Arduino Uno        9         8         10

AltSoftSerial altSerial;

int countit = 0;
char* myStrings[] = {"AT\r\n", "ATI\r\n", "AT+CPIN?\r\n", "AGMI\r\n", "AT+CGMM\r\n", \
"AT+CGSN\r\n","AT+CREG?\r\n", "AT+COPS?\r\n"};


void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for Arduino Serial Monitor to open
  Serial.println("Test Begin");
  altSerial.begin(115200);
}


void loop() {
  char c;
  
  if (altSerial.available()) { // read
    c = altSerial.read();
    Serial.print(c);
    
  } else {
    
    countit += 1;
    if (countit == 10) {
      countit = 0;
    }
    delay(1000);
    altSerial.println(myStrings[countit]);
    delay(100);
  }
}



