/*
   RadioLib nRF24 Receive Example

   This example listens for FSK transmissions using nRF24 2.4 GHz radio module.
   To successfully receive data, the following settings have to be the same
   on both transmitter and receiver:
    - carrier frequency
    - data rate
    - transmit pipe on transmitter must match receive pipe
      on receiver

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// nRF24 has the following connections:
// NSS pin:   10
// CE pin:    2
// IRQ pin:   3
nRF24 nrf = new Module(10, 7, 2);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//nRF24 nrf = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize nRF24
  Serial.print(F("[nRF24] Initializing ... "));
  // carrier frequency:           2400 MHz
  // data rate:                   1000 kbps
  // output power:                -12 dBm
  // address width:               5 bytes
  int state = nrf.begin(2400, 2000, 0, 5);
  //int state = nrf.begin();
  if(state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // set receive pipe 0 address
  // NOTE: address width in bytes MUST be equal to the
  //       width set in begin() or setAddressWidth()
  //       methods (5 by default)
  Serial.print(F("[nRF24] Setting address for receive pipe 0 ... "));
  byte addr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  
  state = nrf.setReceivePipe(0, addr);
  if(state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }
}

void loop() {
  Serial.print(F("[nRF24] Waiting for incoming transmission ... "));

  // you can receive data as an Arduino String
  // NOTE: receive() is a blocking method!
  //       See example ReceiveInterrupt for details
  //       on non-blocking reception method.
  String str;
  int state = nrf.receive(str);

  // you can also receive data as byte array
  /*
    byte byteArr[8];
    int state = nrf.receive(byteArr, 8);
  */

  if (state == ERR_NONE) {
    // packet was successfully received
    Serial.println(F("success!"));

    // print the data of the packet
    Serial.print(F("[nRF24] Data:\t\t"));
    Serial.println(str);

  } else if (state == ERR_RX_TIMEOUT) {
    // timeout occurred while waiting for a packet
    Serial.println(F("timeout!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }
}
