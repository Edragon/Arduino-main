/*
* @file Motor driver HR8833-Test.ino
* @brief HR8833-Test.ino  Motor control program
*
* control motor positive inversion
*
* @author lei.wu@dfrobot.com
* @version  V1.0
* @date  2016-4-13
*/
const int IA1=10;
const int IA2=12;
const int IB1=11;
const int IB2=13;

void setup() {
     pinMode(IA1, OUTPUT);
     pinMode(IA2, OUTPUT);
     pinMode(IB1, OUTPUT);
     pinMode(IB2, OUTPUT);
}

void loop() {
 MA1_Forward(200);//Motor MA1 forward; PWM speed control
 delay(1000);
 MA2_Backward(200);//Motor MA1 backward; PWM speed control
 delay(1000);
}

void MA1_Forward(int Speed1)  //fast decay; Speed = High duty-cycle
{
     analogWrite(IA1,Speed1);
     digitalWrite(IA2,LOW);
  }

void MA2_Backward(int Speed1)  //slow decay; Speed = Low duty-cycle
{
    int Speed2=255-Speed1;
    analogWrite(IA1,Speed2);
    digitalWrite(IA2,HIGH);
  }

void MB1_Forward(int Speed1)
{
     analogWrite(IB1,Speed1);
     digitalWrite(IB2,LOW);
  }

void MB2_Backward(int Speed1)
{
    int Speed2=255-Speed1;
    analogWrite(IB1,Speed2);
    digitalWrite(IB2,HIGH);
  }
