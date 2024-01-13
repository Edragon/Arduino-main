
// Example 6 - Receiving binary data

const byte numBytes = 32;
byte receivedBytes[numBytes];
byte numReceived = 0;

boolean newData = false;

void setup() {
    Serial.begin(4800);
    Serial.println("<Arduino is ready>");
}

void loop() {
    recvBytesWithStartEndMarkers();
    showNewData();
}



void recvBytesWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    byte startMarker = 0xFA;
    byte endMarker = 0xEB;
    byte rb;
   

    while (Serial.available() > 0 && newData == false) {
        rb = Serial.read();

        if (recvInProgress == true) {
            if (rb != endMarker) {
                receivedBytes[ndx] = rb;
                ndx++;
                if (ndx >= numBytes) {
                    ndx = numBytes - 1;
                }
            }
            else {
                receivedBytes[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                numReceived = ndx;  // save the number for use when printing
                ndx = 0;
                newData = true;
            }
        }

        else if (rb == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        Serial.print("This just in (HEX values)... ");
        for (byte n = 0; n < numReceived; n++) {
            Serial.print(receivedBytes[n], HEX);
            Serial.print(' ');
        }
        Serial.println();
        newData = false;
    }
}
