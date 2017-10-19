//*******************************************************************************
//----------------本例程仅供学习使用，未经作者允许，不得用于其他用途。-----------
//------------------------版权所有，仿冒必究！-----------------------------------
//----------------1.开发环境:Arduino IDE-----------------------------------------
//----------------2.测试使用开发板型号：Arduino Leonardo or Arduino UNO R3-------
//----------------3.单片机使用晶振：16M------------------------------------------
//----------------4.淘宝网址：http://xmdzpj.taobao.com---------------------------
//----------------5.作者：真红炎神---------------------------------------------
//*******************************************************************************/

 #define left     'C'
 #define right    'D'
 #define up       'A'
 #define down     'B'
 #define stop1    'F'
char PWM_Left=10;//定义数字接口10（PWM 输出）
//char PWM_Right=28;//定义数字接口28（PWM 输出）
char  buff[3]={0,0,0};
char  i=0;
void setup()
{   
    Serial1.begin(9600);
    pinMode(PWM_Left,OUTPUT);//10口为输出
}

void loop()
{
  if(Serial1.available())
   {
    while (Serial1.available() > 0)//一直接收
     {
        buff[i++]=Serial1.read();//接收三个字节
        delay(10);
     }
      i=0;//重新接收
   
     if((buff[0]=='O')&&(buff[1]=='N'))//前进
     {
       switch(buff[2])
       {
         case up:    digitalWrite(PWM_Left,LOW);break;
         case down:  digitalWrite(PWM_Left,HIGH);break;
         case left:  break; 
         case right: break;
         case stop1: break;
         
         case 1+'0': analogWrite(PWM_Left,25);break;
         case 2+'0': analogWrite(PWM_Left,50);break;
         case 3+'0': analogWrite(PWM_Left,75);break;
         case 4+'0': analogWrite(PWM_Left,100);break;
         case 5+'0': analogWrite(PWM_Left,125);break;
         case 6+'0': analogWrite(PWM_Left,150);break;
         case 7+'0': analogWrite(PWM_Left,175);break;
         case 8+'0': analogWrite(PWM_Left,225);break;
         case 9+'0':analogWrite(PWM_Left,250);break;
       }
     }
   }  
}

       
