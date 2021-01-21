/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

int led = 13;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(led, OUTPUT);  
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  if (sensorValue >= 400) {
      digitalWrite(led, HIGH);  
      delay(100);        // delay in between reads for stability
      Serial.println(sensorValue);
  } 
  digitalWrite(led, LOW); 
  //else {
    //  digitalWrite(led, LOW);  
    //  delay(100);        // delay in between reads for stability
    //  Serial.println(sensorValue);
  //}
  

  
}
