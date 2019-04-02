#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
int pushButton = 5;
int boot = 4;
int call_flag = 0;

int incomingByte = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mySerial.begin(115200);

  pinMode(boot, OUTPUT);
  pinMode(pushButton, INPUT);

  // boot it
  digitalWrite(4, HIGH);    // turn on GPS
  delay(1000);                       // wait for a second
  //GPS();
  Serial.println("init done ..");
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    
    //Serial.print("I received: ");
    //Serial.println(incomingByte, DEC);
  }

  if (incomingByte == 67 && call_flag == 0) {
    call_flag = 1;
    call();
  }
  
  //int buttonState = digitalRead(pushButton);
  //if ( buttonState == 0 && call_flag == 0 ) {
  //  call_flag = 1;
  //if (Serial.read)
  //  call();

  //} else {

  //}
}

void call() {
  Serial.print("calling ...");
  mySerial.println("ATD18576608994;\r\n");
  delay(5000);
  //mySerial.println("ATD18576608994;\r\n");
  //delay(5000);
  //mySerial.println("ATH\r\n");
}

void GPS() {


  mySerial.println("AT\r\n");
  delay(100);
  ShowSerialData();
  delay(1000);

  mySerial.println("AT+CGPSPWR=1\r\n");
  delay(100);
  ShowSerialData();
  delay(1000);

  mySerial.println("AT+CGPSRST=1\r\n");
  delay(100);
  ShowSerialData();
  delay(1000);
}

void ShowSerialData()
{
  while (mySerial.available() != 0)
    Serial.write(mySerial.read());
}
