int dirpin = 2;
int steppin = 5;
int enpin = 8;


void setup()
{
  pinMode(dirpin, OUTPUT);
  pinMode(steppin, OUTPUT);

  pinMode(enpin, OUTPUT);
  digitalWrite(enpin, LOW);
  digitalWrite(dirpin, LOW);     // Set the direction.

}
void loop()
{

  digitalWrite(steppin, LOW);  // This LOW to HIGH change is what creates the
  delayMicroseconds(200);      // This delay time is close to top speed for this
  digitalWrite(steppin, HIGH); // "Rising Edge" so the easydriver knows to when to step.
  delayMicroseconds(500);      // This delay time is close to top speed for this



}
