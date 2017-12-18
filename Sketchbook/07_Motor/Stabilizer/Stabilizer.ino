

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include  "Wire.h"`

//UNO pin map
int ENA=9;
int ENB=11;

int IN1=7;
int IN2=8;

int IN3=10;
int IN4=12;

int MAS,MBS;


/* IMU Data */
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;


double gyroXangle, gyroYangle; // Angle calculate using the gyro only
double compAngleX, compAngleY; // Calculated angle using a complementary filter
double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter
uint8_t i2cData[14]; // Buffer for I2C data
uint32_t timer;
unsigned long lastTime;      

/*************THE KALMAN FILERING ***************/
double P[2][2] = {{ 1, 0 },{ 0, 1 }};
double Pdot[4] ={ 0,0,0,0};
static const double Q_angle=0.001, Q_gyro=0.003, R_angle=0.5,dtt=0.01,C_0 = 1;
double q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
double angle,angle_dot,aaxdot,aax;
double position_dot,position_dot_filter,positiono;

/*-------------Encoder---------------*/

#define LF 0
#define RT 1

int Lduration,Rduration;
boolean LcoderDir,RcoderDir;

const byte encoder0pinA = 2;
const byte encoder0pinB = 5;  // read left wheel speed
byte encoder0PinALast;

const byte encoder1pinA = 3;
const byte encoder1pinB = 4;  // read right wheel speed
byte encoder1PinALast;

int RotationCoder[2];

/*--------------------------------------*/
float k1,k2,k3,k4; //Adjust the PID Parameters
int turn_flag=0;
float move_flag=0;

int pwm;
int pwm_R,pwm_L;
float range;
float range_error_all;
float wheel_speed;
float last_wheel;
float error_a=0;

 
void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    Serial.begin(9600);
	//Serial.println("AT+NAMEelectrodragon");

    //motor contraler
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
  
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(6, OUTPUT);  
 	digitalWrite(6, LOW);//disable buzzer

/******************************************************/
  //Serial.println("Initializing I2C devices...");
  TWBR = ((F_CPU / 400000L) - 16) / 2; // Set I2C frequency to 400kHz

  i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  i2cData[2] = 0x00; // Set Gyro Full Scale Range to ��250deg/s
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ��2g
  while (i2cWrite(0x19, i2cData, 4, false)); // Write to all four registers at once
  while (i2cWrite(0x6B, 0x01, true)); // PLL with X axis gyroscope reference and disable sleep mode

  while (i2cRead(0x75, i2cData, 1));
  if (i2cData[0] != 0x68) { // Read "WHO_AM_I" register
    Serial.print(F("Error reading sensor"));
    while (1);
  }

  delay(20); // Wait for sensor to stabilize

  while (i2cRead(0x3B, i2cData, 6));
  accX = (i2cData[0] << 8) | i2cData[1];
  accY = (i2cData[2] << 8) | i2cData[3];
  accZ = (i2cData[4] << 8) | i2cData[5];


  double roll  = atan2(accX, accZ) * RAD_TO_DEG;

	//The balance PID 
	k1=43;//24.80;
	k2=1.2;//1.4;//9.66;
	k3=-0.2;//-0.88 ;//4.14;
	k4=-0.65;//-0.55;//0.99;

/*************************************************************************************/
   EnCoderInit();
   timer = micros();
   //Serial.println("BOOT..............");
}

/*****************************main loop*********************************/


void loop() 
{
//GET BLUETOOTH DATA FOR CONTROL
  control();
/********************************************************/
  while (i2cRead(0x3B, i2cData, 14));

  accX = ((i2cData[0] << 8) | i2cData[1]);
  //accY = ((i2cData[2] << 8) | i2cData[3]);
  accZ = ((i2cData[4] << 8) | i2cData[5]);
  //gyroX = (i2cData[8] << 8) | i2cData[9];
  gyroY = (i2cData[10] << 8) | i2cData[11];
  //gyroZ = (i2cData[12] << 8) | i2cData[13];

  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();
  
  double roll  = atan2(accX, accZ) * RAD_TO_DEG-move_flag;


  //double gyroXrate = gyroX / 131.0; // Convert to deg/s
  double gyroYrate = -gyroY / 131.0; // Convert to deg/s

  //gyroXangle += gyroXrate * dt; // Calculate gyro angle without any filter
  //gyroYangle += gyroYrate * dt;

/****************************************************/

  Kalman_Filter(roll,gyroYrate);
  
 if(abs(angle)<35){
 // Serial.print("kfAngle=");Serial.print(roll);//Serial.print("\t");
 //Serial.print(",angleAx=");
 //Serial.print(angle); Serial.print("\t");

  pwm_calculate();
  PWMD();
  
  Serial.print("pwm:");
  Serial.print(pwm); Serial.print("\n");
  //set_motor();
  }
  else{
    analogWrite(ENB,0); 
    analogWrite(ENA,0); 
    Serial.print("down!");
  }	
  delay(2);
Serial.print(";\t\n");
}


/*************************************************************************/

void Kalman_Filter(double angle_m,double gyro_m)
{
angle+=(gyro_m-q_bias) * dtt;
Pdot[0]=Q_angle - P[0][1] - P[1][0];
Pdot[1]=- P[1][1];
Pdot[2]=- P[1][1];
Pdot[3]=Q_gyro;
P[0][0] += Pdot[0] * dtt;
P[0][1] += Pdot[1] * dtt;
P[1][0] += Pdot[2] * dtt;
P[1][1] += Pdot[3] * dtt;
angle_err = angle_m - angle;
PCt_0 = C_0 * P[0][0];
PCt_1 = C_0 * P[1][0];
E = R_angle + C_0 * PCt_0;
K_0 = PCt_0 / E;
K_1 = PCt_1 / E;
t_0 = PCt_0;
t_1 = C_0 * P[0][1];
P[0][0] -= K_0 * t_0;
P[0][1] -= K_0 * t_1;
P[1][0] -= K_1 * t_0;
P[1][1] -= K_1 * t_1;
angle+= K_0 * angle_err;
q_bias += K_1 * angle_err;
angle_dot = gyro_m-q_bias;
}


// **************************
// Init the Incoder
// **************************
void EnCoderInit()
{
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  attachInterrupt(LF, LwheelSpeed, RISING);
  attachInterrupt(RT, RwheelSpeed, RISING);
}


// **************************
// Calculate the pwm
// **************************
void pwm_calculate()
{
   unsigned long  now = millis();       // ��ǰʱ��(ms)
   int Time = now - lastTime;

  int range_error;
  
  //Serial.print("  R:");Serial.print(Rduration);
  //Serial.print("  L:");Serial.print(Lduration);
  range+=(Lduration+Rduration)*0.5;
  range*=0.9;
  range_error=Lduration-Rduration;
  range_error_all+=range_error;

  wheel_speed=range-last_wheel;
  last_wheel=range;
  
  pwm=angle*k1+angle_dot*k2+range*k3+wheel_speed*k4;    //use PID tho calculate the pwm
 
  if(pwm>255)                               //Maximum Minimum Limitations
    pwm=255;
  if(pwm<-255)
    pwm=-255;
      //Serial.print(pwm);Serial.print("\t");
      Serial.print(Lduration);Serial.print("\t");
      Serial.print(Rduration);Serial.print("\t\n");
/*    if(turn_flag==0)
    {
     pwm_R=pwm+range_error_all;
     pwm_L=pwm-range_error_all;
    }
    else
    {
        pwm_R=pwm-turn_flag*68;  //Direction PID control
        pwm_L=pwm+turn_flag*68;
        range_error_all=0;     //clean
    }
 */   
     Lduration = 0;//clean
     Rduration = 0;
   lastTime = now;
   //Serial.print(Time);Serial.print("\n");

  
}

/****************control the motor****************/
void PWMD()
{  
  if(pwm>0)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);    
    Serial.print("...");
  }
  else if(pwm<0)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
   Serial.print("***");
  }
  int PWMr = abs(pwm);
  int PWMl = abs(pwm);

  analogWrite(ENB, PWMl); //PWM����a==0-255
  analogWrite(ENA, PWMr ); //PWM����a==0-255
}





// *******************************
// Read the Speed from the Encoder
// *******************************

void LwheelSpeed()
{
	if(digitalRead(encoder0pinB))
		Lduration++;
	else Lduration--;
/*  int Lstate = digitalRead(encoder0pinA);
  if((encoder0PinALast == LOW)&&Lstate==HIGH)
  {
    int val = digitalRead(encoder0pinB);
    if(val == LOW && LcoderDir)  LcoderDir = false; //Lreverse
    else if(val == HIGH && !LcoderDir)  LcoderDir = true;  //Lforward
  } 
  encoder0PinALast = Lstate;

  if(!LcoderDir)  Lduration++;
  else  Lduration--;
*/   
}


void RwheelSpeed()
{
	if(digitalRead(encoder1pinB))
		Rduration--;
	else Rduration++;
/*  int Rstate = digitalRead(encoder1pinA);
  if((encoder1PinALast == LOW)&&Rstate==HIGH)
  {
    int val = digitalRead(encoder1pinB);
    if(val == LOW && RcoderDir)  RcoderDir = false; //Rreverse
    else if(val == HIGH && !RcoderDir)  RcoderDir = true;  //Rforward
  }
  encoder1PinALast = Rstate;

  if(!RcoderDir)  Rduration--;
  else  Rduration++;
*/  
}

void control()
{
	if(Serial.available()){
		int val;
		val=Serial.read();
		switch(val){
			case 'F':
				if(move_flag<5)
					move_flag+=0.1;
				Serial.println("forword");
				break;
			case 'B':
				if(move_flag>-5)move_flag-=0.1;
				Serial.println("back");
				break;
			case 'S':
				move_flag=0;
				turn_flag=0;
				Serial.println("stop");
				break;
			default:
				break;

		}
		}
}

