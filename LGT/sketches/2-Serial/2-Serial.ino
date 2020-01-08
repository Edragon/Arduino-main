#include <SoftwareSerial.h>

SoftwareSerial ms(5, 6); // RX, TX
SoftwareSerial bt(9, 7); // RX, TX

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(PB7, OUTPUT);
  pinMode(11, OUTPUT);
  ms.begin(9600);
  bt.begin(9600);

}


void loop() {
  digitalWrite(11, LOW);

  //  bt.println("987654");
  //  ms.println("987654");
  //  delay(1000);
  ms.listen();

  //  if (bt.available()) {
  //    ms.write(bt.read());
  //  }


  while (ms.available() > 0) {
    char inByte = ms.read();
    bt.write(inByte);
  }

  //  if (ms.available()) {
  //    bt.write(ms.read());
  //  }


}
