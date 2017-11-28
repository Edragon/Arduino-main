void read_analog() {
  int sen_0 = analogRead(A0);
  voltage_0 = sen_0 * (5.0 / 1023.0);

  int sen_1 = analogRead(A1);
  voltage_1 = sen_1 * (5.0 / 1023.0);

  int sen_2 = analogRead(A2);
  voltage_2 = sen_2 * (5.0 / 1023.0);
}
void read_button() {
  int sbtn = analogRead(A3);
  btn = sbtn * (5.0 / 1023.0);
  if (btn <= 4.6 and btn >= 4.2) {
    pressed_btn = 1;
  } else if ( btn <= 4.2) {
    pressed_btn = 2;
  } else {
    pressed_btn = 0;
  }
}

void update_display() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);

  display.print("EMS-0: ");
  display.println(voltage_0);
  display.print("EMS-1: ");
  display.println(voltage_1); 
  display.print("EMS-2: ");
  display.println(voltage_2);

  display.print("button value: ");
  display.println(pressed_btn);
  
  display.display();


  delay(1000);



}

