#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
int pushButton = 5;
int boot = 4;
int REG_flag = 0;
int GPS_flag = 0;
int status_LED = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mySerial.begin(115200);

  pinMode(boot, OUTPUT);
  pinMode(pushButton, INPUT);
  pinMode(status_LED, OUTPUT);

  // boot it
  digitalWrite(4, HIGH);    // turn on GPS
  delay(2000);                       // wait for a second
  //GPS();
  Serial.println("init done ..");
  delay(1000);
}

void loop() {
  mySerial.println("AT+CPIN?\r\n");
  if  (mySerial.find("READY") ) {
    Serial.println("Found READY!");
    blink_LED();

  }
  mySerial.flush();
  delay(1000);

  mySerial.println("AT+CREG?\r\n");
  if  (mySerial.find("1") && REG_flag == 0 ) {
    Serial.println("Found +CREG: 0,1!");

    //blink_LED();
    REG_flag = 1;
  }
  mySerial.flush();
  delay(1000);

  int buttonState = digitalRead(pushButton);
  if ( buttonState == 0 && REG_flag == 1 ) {
    REG_flag = 2;
    call();

  }

  if ( buttonState == 1 && GPS_flag == 0) {
    GPS_flag = 1;
    GPS();
  }
}


void blink_LED() {
  digitalWrite(13, HIGH);
  delay(50);
  digitalWrite(13, LOW);
  delay(50);
  digitalWrite(13, HIGH);
  delay(50);
  digitalWrite(13, LOW);
  delay(50);  
  digitalWrite(13, HIGH);
  delay(50);
  digitalWrite(13, LOW);
  delay(50);  
  digitalWrite(13, HIGH);
  delay(50);
  digitalWrite(13, LOW);
  delay(50);  
  digitalWrite(13, HIGH);
  delay(50);
  digitalWrite(13, LOW);
  delay(50);
}

void call() {
  Serial.println("calling ...");
  mySerial.println("ATD18576608994;\r\n");
  delay(2000);
  //mySerial.println("ATD18576608994;\r\n");
  //delay(5000);
  //mySerial.println("ATH\r\n");
  mySerial.flush();
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
  Serial.println("turned on GPS ...");
}

void ShowSerialData()
{
  while (mySerial.available() != 0)
    Serial.write(mySerial.read());
}
