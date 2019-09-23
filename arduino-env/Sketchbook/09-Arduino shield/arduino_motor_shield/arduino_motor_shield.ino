long frequency = 500; //频率, 单位Hz

//Motor A
int PWMA = 5; //Speed control
int AIN1 = 4; //Direction
int AIN2 = 7; //Direction

//Motor B
int PWMB = 6; //Speed control
int BIN1 = 8; //Direction
int BIN2 = 12; //Direction

void setup() {
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  init2();
  init2();
  init2();
  init2();
  init2();
}

void loop() {
  move(1, 255, 1); //motor 1, full speed, left
  move(2, 255, 1); //motor 2, full speed, left

  delay(5000); //go for 1 second

  move(1, 200, 0); //motor 1, half speed, right
  move(2, 200, 0); //motor 2, half speed, right

  delay(5000);
}

void init2() {
  digitalWrite(13, HIGH);
  tone(3, frequency );
  delay(500);
  digitalWrite(13, LOW);
  noTone(3);
  delay(500);
}




void move(int motor, int speed, int direction) {
  //Move specific motor at speed and direction
  //motor: 0 for B 1 for A
  //speed: 0 is off, and 255 is full speed
  //direction: 0 clockwise, 1 counter-clockwise


  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if (direction == 1) {
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if (motor == 1) {
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  } else {
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}
