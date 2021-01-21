
void analog_read() {
  f_volt = analogRead(A0);
  i_volt = analogRead(A0);
  
  Serial.print("float read: ");
  Serial.println(f_volt);
  Serial.print("int read: ");
  Serial.println(i_volt);
  delay(1000);
}

