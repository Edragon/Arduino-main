
#define pulse_ip 7
int ontime, offtime, duty;
float freq, period;

void setup()
{
  pinMode(pulse_ip, INPUT);
  Serial.begin(9600);
}
void loop()
{
  ontime = pulseIn(pulse_ip, HIGH);
  offtime = pulseIn(pulse_ip, LOW);
  period = ontime + offtime;
  freq = 1000000.0 / period;
  duty = (ontime / period) * 100;

  
  Serial.print(" - ");
  Serial.print(period);
  Serial.print(" - ");
  Serial.print(duty);
  Serial.print(" - ");
  Serial.println(freq);
  delay(100);
}
