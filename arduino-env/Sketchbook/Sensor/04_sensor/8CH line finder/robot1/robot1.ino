#include <QTRSensors.h>
//电机驱动器针
#define out_STBY 6
#define out_B_PWM 9
#define out_A_PWM 3
#define out_A_IN2 4
#define out_A_IN1 5
#define out_B_IN1 7
#define out_B_IN2 8
#define motor_A 0
#define motor_B 1
#define motor_AB 2

#define NUM_SENSORS 8 //使用的传感器数量
#define TIMEOUT 2500 // 等待2500我们的传感器输出变为低电平 - aspetta2500我们初步二mandare低我sensori
  //定义QTR pololu传感器
QTRSensorsRC qtrrc((unsigned char[]) {2, 12, 11, 10, 17, 16, 15, 14}
 , NUM_SENSORS, TIMEOUT, 255);
unsigned int sensors[NUM_SENSORS];
  void setup()
{
  Serial.begin(115200);
  // 定义电机驱动输出引脚 - definisci i引脚输出迪德尔MOTORE
  pinMode(out_STBY,OUTPUT);
  pinMode(out_A_PWM,OUTPUT);
  pinMode(out_A_IN1,OUTPUT);
  pinMode(out_A_IN2,OUTPUT);
  pinMode(out_B_PWM,OUTPUT);
  pinMode(out_B_IN1,OUTPUT);
  pinMode(out_B_IN2,OUTPUT);
  motor_standby(false);// motor stand-by off and stop - togli i motori dallo stand-by e fermali
  stopMotors();
  delay(2000);
  SensorCalibration(); // sensor calibration - calibra i sensori
  delay(2000);
}

void loop()
{
  follow_segment(); // follow the black line - segui la linea nera
}
// PID parameters
int Kp=120;
int Ki=100;
int Kd=100;
// follow the black line - segui la linea nera 
void follow_segment()
{
  int last_proportional = 0;
  long integral=0;
  int primo=0;
  int posizione = qtrrc.readLine(sensors); // read the IR sensors - leggi i sensori IR 
  long int proportional = posizione;
  int P = proportional * Kp/1000; // Proportional parameter 
  long int derivative = proportional - last_proportional; //Derivative parameter
  if (primo==0) // firt cycle set derivative to zero - al primo giro imposta il derivativo a zero
     {
      primo=1;
      derivative=0;
    }
  int D=derivative*Kd/1000;
  integral += proportional;
  int I=integral*Ki/1000; //integral parameter 
  // Remember the last position - ricorda l'ultima posizione
  last_proportional = proportional;
  int PID =P + I + D; //calculate the PID 
  // print some values on the serial port - scrivi qualche valore sulla seriale
  unsigned char zz;
   for (zz = 0; zz < NUM_SENSORS; zz++)
    {
      Serial.print(sensors[zz]);
      Serial.print(' ');
    }
    Serial.print(" position: ");
    Serial.print(posizione);
    Serial.print(" P:");
    Serial.print(P);
    Serial.print(" I:");
    Serial.print(I);
    Serial.print(" D:");
    Serial.print(D);
    Serial.print(" PID:");
    Serial.print(PID);
  // set the values for the motors based on the PID gain - setta i valori del motore basati sul PID
    const int maximum = 40; // the maximum speed
     if (PID > 0){
       if (PID > maximum) PID=maximum;
       motor_speed2(motor_B,maximum);
       motor_speed2(motor_A,maximum-PID);
      Serial.print(" Motore_B:");     Serial.print(maximum);
    Serial.print(" Motore_A:");      Serial.print(maximum-PID);
      }
      else {
       if (PID < -maximum) PID=-maximum;
       motor_speed2(motor_B,maximum+PID);
        motor_speed2(motor_A,maximum);
      Serial.print(" Motore_B:");     Serial.print(maximum+PID);
    Serial.print(" Motore_A:");      Serial.print(maximum);
      }
      Serial.println("");
}
// ------------------ stop the robot - ferma il robottino ----------------------- 
void stopMotors(){
  motor_brake(motor_A);
  motor_brake(motor_B);
}
// ------------------ Motor driver mgmt routines - routine per gestire i motori ----------------------- 
void motor_speed2(boolean motor, char speed) { //speed from 0 to 100
  if (motor == motor_A)
   speed = -speed;
  byte PWMvalue=0;
  PWMvalue = map(abs(speed),0,100,50,255); //anything below 50 is very weak
  if (speed > 0)
    motor_speed(motor,0,PWMvalue);
  else if (speed < 0)
    motor_speed(motor,1,PWMvalue);
  else {
    motor_coast(motor);
  }
}
void motor_speed(boolean motor, boolean direction, byte speed) { //speed from 0 to 255
  if (motor == motor_A) {
    if (direction == 0) {
      digitalWrite(out_A_IN1,HIGH);
      digitalWrite(out_A_IN2,LOW);
    } else {
      digitalWrite(out_A_IN1,LOW);
      digitalWrite(out_A_IN2,HIGH);
    }
    analogWrite(out_A_PWM,speed);
  } else {
    if (direction == 0) {
      digitalWrite(out_B_IN1,HIGH);
      digitalWrite(out_B_IN2,LOW);
    } else {
       digitalWrite(out_B_IN1,LOW);
      digitalWrite(out_B_IN2,HIGH);
    }
    analogWrite(out_B_PWM,speed);
  }
}
// stand-by motor
void motor_standby(boolean state) { //low power mode
  if (state == true)
    digitalWrite(out_STBY,LOW);
  else
    digitalWrite(out_STBY,HIGH);
}
// stop the motors
void motor_brake(boolean motor) {
  if (motor == motor_A) {
    digitalWrite(out_A_IN1,HIGH);
    digitalWrite(out_A_IN2,HIGH);
  } else {
    digitalWrite(out_B_IN1,HIGH);
    digitalWrite(out_B_IN2,HIGH);
  }
}
// motors coast - fai girare i motori in folle
void motor_coast(boolean motor) {
  if (motor == motor_A) {
    digitalWrite(out_A_IN1,LOW);
    digitalWrite(out_A_IN2,LOW);
    digitalWrite(out_A_PWM,HIGH);
  } else {
    digitalWrite(out_B_IN1,LOW);
     digitalWrite(out_B_IN2,LOW);
    digitalWrite(out_B_PWM,HIGH);
  }
}
// IR sensor calibration 
void SensorCalibration() {
  int counter, i;
  for (counter=0; counter<40; counter++)
  {
    if (counter < 10 || counter >= 30) {
      motor_speed2(motor_A,10);
      motor_speed2(motor_B,-10);
    }
    else   {
      motor_speed2(motor_A,-10);
      motor_speed2(motor_B,10);
    }
    qtrrc.calibrate();
    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
     // print the calibration minimum values measured when emitters were on
    for (i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
    // print the calibration maximum values measured when emitters were on
  for (i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  stopMotors();
  delay(2000);
}
