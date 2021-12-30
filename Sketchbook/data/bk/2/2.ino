
String getAscii = "3132333435363738" //get this string data from serial port.

int d = getAscii.length();

for (int i = 0; i <= d; i = i + 2)
{
  String c = getAscii.substring(i, i + 2);
  int b = c.toInt() + 18;
  lcd.write(b);
  delay(100);
}
