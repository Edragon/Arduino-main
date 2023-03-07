
#include <SoftwareSerial.h>
#include <String.h>

SoftwareSerial mySerial(2, 3);

void setup()
{
  mySerial.begin(9600);               // the GPRS baud rate
  Serial.begin(9600);    // the GPRS baud rate
  delay(500);
  Serial.println("Here is the menu for pre-wrote function:");
  Serial.println("T for texting a message");
  Serial.println("D for making a calle");
  Serial.println("H for hanging the call");
  Serial.println("O for turn on GPS");
  Serial.println("G for check GPS status");
  Serial.println("^^feel free to modify the code");
}

void loop()
{
  //after start up the program, you can using terminal to connect the serial of gprs shield,
  //if you input 't' in the terminal, the program will execute SendTextMessage(), it will show how to send a sms message,
  //if input 'd' in the terminal, it will execute DialVoiceCall(), etc.

  if (Serial.available())
    switch (Serial.read())
    {
      case 'T':
        SendTextMessage();
        break;
      case 'D':
        DialVoiceCall();
        break;
      case 'H':
        HangVoiceCall();
        break;
      case 'O':
        GPSON();
        break;
      case 'G':
        GPS();
        break;

    }
  if (mySerial.available())
    Serial.write(mySerial.read());
}

///SendTextMessage()
///this function is to send a sms message
void SendTextMessage()
{
  mySerial.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode
  delay(300);
  mySerial.println("AT+CMGS=\"18576608994\"");//send sms message, be careful need to add a country code before the cellphone number
  delay(300);
  mySerial.println("How are you2");//the content of the message
  delay(300);
  mySerial.println((char)26);//the ASCII code of the ctrl+z is 26
  delay(300);
  mySerial.println();
}

///DialVoiceCall
///this function is to dial a voice call
void DialVoiceCall()
{
  mySerial.println("ATD13725585994;");//dial the number
  delay(100);
  mySerial.println();
}

void HangVoiceCall()
{
  mySerial.println("ATH\r");//dial the number
  delay(100);
  mySerial.println();
}

void ShowSerialData()
{
  while (mySerial.available() != 0)
    Serial.write(mySerial.read());
}

void GPSON()
{
  mySerial.println("AT+CGPSPWR=1\r");//dial the number
  delay(1000);
  mySerial.println();
  mySerial.println("AT+CGPSRST=1\r");//dial the number
  delay(1000);
  mySerial.println();
}
void GPS()
{
  mySerial.println("AT+CGPSINF=0\r");//dial the number
  delay(1000);
  mySerial.println();
  mySerial.println("AT+CGPSSTATUS?");//dial the number
  delay(100);
  ShowSerialData();
  delay(1000);
}
