const uint8_t frqpin = 2; // digital pin #5
const uint32_t oneSecond = 1000;
uint32_t timer = 0;
uint32_t sts = 0;
const uint32_t c = 3000; // wait for 3000 pulses
uint32_t ets = 0;




void setup() {
 Serial.begin(9600);
 pinMode(frqpin,INPUT);
 digitalWrite(frqpin, HIGH); // pullup
}


void loop() {
 pulseIn(frqpin,LOW);
 sts = micros(); // start time stamp
  for (uint32_t i=c; i>0; i--)
   pulseIn(frqpin,HIGH);
 ets = micros(); // end time stamp
 Serial.print("$");
 Serial.println((c*1e6/(ets-sts))); // output Hz
}
