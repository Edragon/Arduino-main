#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);
unsigned char card[12];
void setup()
{
Serial.begin(9600);
lcd.print(“Card ID :”);
}
 
void loop()
{
unsigned char i=0;
for(;;)
{
if (Serial.available()>0)
{
card[i]=Serial.read();
i++;
}
if (i>=11)
{
i=0;
decode();
}
}
}
void decode()
{
lcd.setCursor(0, 1);
unsigned char p;
for(p=3;p<11;p++)
{
lcd.print(card[p]);
}
}
