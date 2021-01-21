unsigned long time;
int map_val = 0;

void setup() {
  Serial.begin(9600);
}
void loop() {
  Serial.print("Time: ");
  time = millis();
  map_val = map(time, 0, 100000, 2000,10000);
  
  Serial.println(time); //prints time since program started
  Serial.println(map_val); //prints time since program started
  
  delay(1000);          // wait a second so as not to send massive amounts of data

}
