int test[] = {0x06, 0xE0, 0xB8};

long test2;

long l = 135274560L;

void setup() {
  test2 = long(test[0]) * 256L * 256L;
  Serial.begin(9600);
}

void loop() {
  delay(1000);
  //Serial.println(l);
  Serial.println(test2);
}

//void loop() {
//  Serial.write ((char *)&l, 4);
//  delay(1000);
//  Serial.println("");
//}

//void loop() {
//  Serial.write(l);
//  Serial.write(l >> 7);
//  Serial.write(l >> 14);
//  Serial.write(l >> 21);
//}
