
// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include<Wire.h>
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

const int PWM_A   = 9;
const int DIR_A   = 7;
const int BRAKE_A = 8;

const int PWM_B   = 10;
const int DIR_B   = 11;
const int BRAKE_B = 12;

const int beep = 6;

bool Break = true;
bool DIR = true;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);

  pinMode(6, OUTPUT);
}


void read_6050() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers

  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);
  Serial.print(" | Tmp = "); Serial.print(Tmp / 340.00 + 36.53); //equation for temperature in degrees C from datasheet
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);
  delay(333);
}

void short_beep() {
  digitalWrite(beep, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(beep, LOW);    // turn the LED off by making the voltage LOW
  delay(100);                       // wait for a second
  digitalWrite(beep, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(beep, LOW);    // turn the LED off by making the voltage LOW
  delay(100);
}

void full_speed_forward_A() {

  digitalWrite(BRAKE_A, LOW);
  digitalWrite(DIR_A, HIGH);
  analogWrite(PWM_A, 255);
  delay(500);
}

void full_speed_backward_A() {
  digitalWrite(BRAKE_A, HIGH);
  digitalWrite(DIR_A, LOW);
  analogWrite(PWM_A, 255);
  delay(500);
}

void break_A() {
  digitalWrite(BRAKE_A, HIGH);
  digitalWrite(DIR_A, HIGH);
  delay(200);
}


void loop() {
  // put your board flat on table, Acx will be around 1000;
  read_6050(); // sen your attitude;

  if ( AcX > 10000 and Break == true ) {
    // change direction and break first;
    DIR = false;
    Break = false;
    short_beep();
    Serial.println("Break and turn direction now!");
    break_A();
    short_beep();
    Serial.println("Full speed backward! ");
    full_speed_backward_A();
  }

  else if ( AcX > 10000 and Break == false ) {
    full_speed_backward_A();
  }

  else if  ( AcX < 10000 and DIR == true) {
    full_speed_forward_A();
  }

  else if  ( AcX < 10000 and DIR == false) {
    Serial.println("Full speed forward! ");
    DIR = true;
    short_beep();
    full_speed_forward_A();
  }
}
