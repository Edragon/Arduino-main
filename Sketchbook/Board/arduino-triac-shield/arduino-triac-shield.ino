#define signal_pin 2  // Modify this pin as per your connection
#define status_pin 13
void setup() {
  pinMode(relay_pin , OUTPUT);
  digitalWrite(relay_pin, LOW);

  pinMode(signal_pin , INPUT_PULLUP);

  Serial.begin(9600);
}
void loop() {

  if ( digitalRead(signal_pin) == 0 ) {
    Serial.println (" AC Mains 230v Detected ");
    digitalWrite(status_pin, HIGH);
  } else {
    Serial.println (" Power OFF ");
    digitalWrite(status_pin, LOW);
  }

}
