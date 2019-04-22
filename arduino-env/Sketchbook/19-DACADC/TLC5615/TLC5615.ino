#include <TLC5615.h>

TLC5615 dac(10);

void setup(void)
{
  dac.begin();
}

void loop(void)
{
  dac.analogWrite(1000); // 3.99V
  delay(2000);
  dac.analogWrite(500); // 1.99V
  delay(2000);
}
