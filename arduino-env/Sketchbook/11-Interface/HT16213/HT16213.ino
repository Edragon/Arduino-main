/**
   \file HT1621-test.ino
   \brief Simple test sketch for the HT1621 class.
   \author Enrico Formenti
   \date 4 february 2015
   \version 1.0
   \copyright BSD license, check the License page on the blog for more information. All this text must be
    included in any redistribution.
    <br><br>
    See macduino.blogspot.com for more details.
*/

#include "HT16213.h"

// refere to Macduino website for pin connections and their meaning
HT1621 ht(2, 3, 4);  

void setup() {



  ht.begin();

  ht.sendCommand(HT1621::RC256K);
  ht.sendCommand(HT1621::BIAS_THIRD_4_COM);
  ht.sendCommand(HT1621::SYS_EN);

  ht.sendCommand(HT1621::LCD_OFF);
  delay(1000);
  ht.sendCommand(HT1621::LCD_ON);
  delay(1000);
  ht.sendCommand(HT1621::LCD_OFF);
  delay(1000);
  ht.sendCommand(HT1621::LCD_ON);
  delay(1000);
}

void loop() {

  register uint8_t i;

  // clear memory
  for (i = 0; i < 128; i++)
    ht.write(i, 0x00);
  delay(2000);


  // write all zeroes
  for (i = 0; i < 128; i++)
    ht.write(i, 0x3E);
  delay(2000);


  // write all zeroes
  for (i = 0; i < 128; i++)
    ht.write(i, 0xff);
  delay(2000);
}
