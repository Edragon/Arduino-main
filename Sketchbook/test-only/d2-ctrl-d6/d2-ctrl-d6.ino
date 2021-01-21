#define signal_Pin 13  // Modify this pin as per your connection
#define relay_pin 6
void setup() {
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);

  pinMode(signal_Pin, INPUT);

  Serial.begin(9600);
}
void loop() {

  if ( digitalRead(signal_Pin) == 1 ) {
    Serial.println (" AC Mains 230v Detected ");
    digitalWrite(relay_pin, HIGH);
  } else {
    Serial.println (" Power OFF ");
    digitalWrite(relay_pin, LOW);
  }

}
