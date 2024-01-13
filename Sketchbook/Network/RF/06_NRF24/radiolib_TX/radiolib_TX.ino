/*
   RadioLib nRF24 Transmit Example

   This example transmits packets using nRF24 2.4 GHz radio module.
   Each packet contains up to 32 bytes of data, in the form of:
    - Arduino String
    - null-terminated char array (C-string)
    - arbitrary binary data (byte array)

   Packet delivery is automatically acknowledged by the receiver.

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
  if(state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // set transmit address
  // NOTE: address width in bytes MUST be equal to the
  //       width set in begin() or setAddressWidth()
  //       methods (5 by default)
  byte addr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  
  Serial.print(F("[nRF24] Setting transmit pipe ... "));
  state = nrf.setTransmitPipe(addr);
  if(state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }
}

void loop() {
  Serial.print(F("[nRF24] Transmitting packet ... "));

  // you can transmit C-string or Arduino string up to
  // 32 characters long
  int state = nrf.transmit("Hello World!");

  if (state == ERR_NONE) {
    // the packet was successfully transmitted
    Serial.println(F("success!"));

  } else if (state == ERR_PACKET_TOO_LONG) {
    // the supplied packet was longer than 32 bytes
    Serial.println(F("too long!"));

  } else if (state == ERR_ACK_NOT_RECEIVED) {
    // acknowledge from destination module
    // was not received within 15 retries
    Serial.println(F("ACK not received!"));

  } else if (state == ERR_TX_TIMEOUT) {
    // timed out while transmitting
    Serial.println(F("timeout!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }

  // wait for a second before transmitting again
  delay(1000);
}
