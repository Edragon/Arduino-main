
void volt_read()
{
  char pre_volt[] = "analog value char: ";
  char volt_char[] = "";

  dtostrf(f_volt, 6, 2, volt_char);         // data to string

  sprintf(data_volt, "%s %s", pre_volt, volt_char);   // combine char array

  Serial.println(data_volt);
  delay(1000);        // delay in between reads for stability
  return data_volt;
}


void rad_mydevice() {
  char my_device[] = "";
  char devic_prefix[5] = "ESP_";
  int rad = random(1000, 9999);

  sprintf(my_device, "%s%d", devic_prefix, rad);
  Serial.println(my_device);
  delay(1000);
}

