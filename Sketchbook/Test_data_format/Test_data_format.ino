float f_volt;
int i_volt;

char data_volt[80];
String combine_str;

void setup() {
  Serial.begin(9600);

}

void loop() {
  rad_mydevice();
  
  int_to_char();
  
  analog_read();

  //handle char
  volt_read();
  
  //handle string
  str_handler();
  
  //string to char array
  str_tochar();

  find_str();
}
