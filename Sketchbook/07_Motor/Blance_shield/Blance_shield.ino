

const int PWM_A   = 9;
const int DIR_A   = 7;
const int BRAKE_A = 8;

const int PWM_B   = 10;
const int DIR_B   = 11;
const int BRAKE_B = 12;

const int beep = 6;
//const int SNS_A   = A0;


void setup() {
  // Configure the A output
  pinMode(BRAKE_A, OUTPUT);  // Brake pin on channel A
  pinMode(DIR_A, OUTPUT);    // Direction pin on channel A
  pinMode(beep, OUTPUT);    // Direction pin on channel A

  // Open Serial communication
  Serial.begin(9600);
  Serial.println("Motor shield DC motor Test:\n");
}

void short_beep() {
  digitalWrite(beep, HIGH);
  delay(100);
  digitalWrite(beep, LOW);
}

void loop() {

  short_beep();
  // Set the outputs to run the motor forward
  Serial.println("1. current consumption at full speed: ");
  digitalWrite(BRAKE_A, LOW);  // setting brake LOW disable motor brake
  digitalWrite(DIR_A, HIGH);   // setting direction to HIGH the motor will spin forward
  analogWrite(PWM_A, 255);     // Set the speed of the motor, 255 is the maximum value
  delay(5000);                 // hold the motor at full speed for 5 seconds

  //Serial.println(analogRead(SNS_A));


  short_beep();
  // Brake the motor
  Serial.println("2. Start braking");
  // raising the brake pin the motor will stop faster than the stop by inertia
  digitalWrite(BRAKE_A, HIGH);  // raise the brake
  delay(2000);

  short_beep();
  // Set the outputs to run the motor backward
  Serial.println("3.current consumption backward: ");
  digitalWrite(BRAKE_A, HIGH);  // setting againg the brake LOW to disable motor brake
  digitalWrite(DIR_A, LOW);    // now change the direction to backward setting LOW the DIR_A pin
  analogWrite(PWM_A, 255);     // Set the speed of the motor
  delay(5000);


  short_beep();
  // now stop the motor by inertia, the motor will stop slower than with the brake function
  analogWrite(PWM_A, 0);       // turn off power to the motor

  //Serial.print("current brake: ");
  //Serial.println(analogRead(A0));
  Serial.println("4. End of the motor shield test with DC motors. Thank you!");


  while (1);
}
