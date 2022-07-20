

#include <SPIFlash_lp.h>    //get it here: https://github.com/LowPowerLab/SPIFlash
#include <SPI.h>

// 0x1F44 for adesto(ex atmel) 4mbit flash
// 0xEF30 for windbond 4mbit flash
// 0xEF40 for windbond 64mbit flash

SPIFlash flash(8, 0xEF30);

void setup(){
  Serial.begin(115200);
  Serial.print("Start...");

  if (flash.initialize())
  {
    Serial.println("Init OK!");
  }
  else
    Serial.println("Init FAIL!");
  
  delay(1000);
}

void loop(){

}
