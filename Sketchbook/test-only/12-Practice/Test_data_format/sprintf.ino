// sprintf
void int_to_char() {
  int i = 12345;
  char buf[6];
  sprintf(buf, "%d", i);
  Serial.print("buf get from int 12345:   ");
  Serial.println(buf);
  delay(1000);
}

