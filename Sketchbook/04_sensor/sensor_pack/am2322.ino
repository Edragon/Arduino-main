void am2322_print() {
  // READ DATA
  Serial.print("AM2322: ");
  int status = AM2322.read();
  
  switch (status)
  {
  case AM232X_OK:
    Serial.print("OK");
    break;
    
  default:
    Serial.print(status);
    Serial.print("\t");
    break;
  }
  
  // DISPLAY DATA, sensor only returns one decimal.
  Serial.print("; A_HU: ");
  
  Serial.print(AM2322.humidity, 1);
  Serial.print("; A_TE: ");
  //Serial.print(",\t");
  Serial.print(AM2322.temperature/10, 2);
  Serial.print("; ");
}


