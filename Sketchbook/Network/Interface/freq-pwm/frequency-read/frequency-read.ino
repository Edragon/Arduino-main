//#include <LiquidCrystal.h>
//
//LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

#define pulse_ip 2
int ontime,offtime,duty;
float freq,period;
   
void setup()
{
  pinMode(pulse_ip, INPUT);
  Serial.begin(9600);
  
//  lcd.begin(16, 4);
//  lcd.clear();
//  lcd.print("Freq:");
//  lcd.setCursor(0,1);
//  lcd.print("Ton:");
//  lcd.setCursor(0,2);
//  lcd.print("Toff:");  
//  lcd.setCursor(0,3);
//  lcd.print("Duty:");  
}

void loop()
{
   ontime = pulseIn(pulse_ip,HIGH);
   offtime = pulseIn(pulse_ip,LOW);
   period = ontime+offtime;
   freq = 1000000.0/period;
   duty = (ontime/period)*100; 
   
   Serial.print("T_on: "); Serial.print(ontime); Serial.print("us ");
   Serial.print("T_off: "); Serial.print(offtime); Serial.print("us ");
   Serial.print("freq: "); Serial.print(freq); Serial.print("Hz ");
   Serial.print("duty: "); Serial.print(duty); Serial.println("% ");
   delay(1000);
      
//   lcd.setCursor(4,1);
//   lcd.print(ontime);
//   lcd.print("us");
//   lcd.setCursor(5,2);
//   lcd.print(offtime); 
//   lcd.print("us");   
//   lcd.setCursor(5,0); 
//   lcd.print(freq);
//   lcd.print("Hz");
//   lcd.setCursor(6,3);
//   lcd.print(duty); 
//   lcd.print('%'); 

}
