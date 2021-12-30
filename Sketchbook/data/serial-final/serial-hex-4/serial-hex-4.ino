#include <SoftwareSerial.h>

SoftwareSerial mySerial(6, 7); // RX, TX

// 012345678901234567890123
// demo data from serial
// F2 5A 02 E0 B8  07 34 2D 00 3E  67 07 22 DC 4D  ED 38 DF EC 33  61 00 00 7D
// F2 5A 02 E0 B8 07 34 2D 00 3E 67 07 22 DC 4D ED 38 DF EC 33 61 00 00 7D
void setup() {
  Serial.begin(4800); // HLW8032

  mySerial.begin(4800);

}

void loop() {
  char hex[] = {};
   memset(hex, 0, 24);
   
  if (Serial.available() > 0) {
    
    for (int i = 0; i < 24; i++) {
      //delay(20);
      char inChar = (char)Serial.read();
      hex[i] = inChar;
      
    }
  }
  
  for (int i = 0; i < 24; i++) { 
    mySerial.write (hex[i]);
  }
  
  //mySerial.print( hex );
  delay (1000);

  
}
