
// zoomkat 10-29-10 simple delimited ',' string parce
// from serial port input (via serial monitor)
// and print result out serial port
// CR/LF could also be a delimiter
// for IDE 0019 and later

String readString;

void setup() {
  Serial.begin(9600);
  delay(1000);
  byte message[] = {0xFA, 0xFD};
  //byte message2[] = {0xFA, 0xF5};
  // put your main code here, to run repeatedly:
  Serial.write(message, sizeof(message));
  delay(1000);
  Serial.write(0xFA);
  Serial.write(0xFD);
  //Serial.write(message2, sizeof(message2));
  delay(1000);
}

void loop() {

  //expect a string like wer,qwe rty,123 456,hyre kjhg,
  //or like hello world,who are you?,bye!,
  while (Serial.available()) {
    delay(10);  //small delay to allow input buffer to fill

    char c = Serial.read();  //gets one byte from serial buffer
    if (c == 'FF') {
      break;
    }  //breaks out of capture loop to print readstring
    readString += c;
  } //makes the string readString

  if (readString.length() > 0) {
    Serial.println(readString); //prints string to serial port out

    readString = ""; //clears variable for new input
  }
}
