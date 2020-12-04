#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX

char receivedChar;
boolean newData = false;

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);
    
    Serial.println("<Arduino is ready>");
}

void loop() {
    recvOneChar();
    showNewData();
}

void recvOneChar() {
    if (mySerial.available() > 0) {
        receivedChar = mySerial.read();
        newData = true;
    }
}

void showNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChar, HEX);  // may not need HEX
        newData = false;
    }
}
