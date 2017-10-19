#include <EEPROM.h>
int addr_1 = 0;
int addr_2 = 1;
//int val_1;
//int val_2;
int check_value = 100;

int btn = 0;

void btnIRQ();

void setup() {
  btnIRQ();
  // put your setup code here, to run once:
  EEPROM.begin(512);
  Serial.begin(115200);
}

void loop() {

  int value_1 = EEPROM.read(addr_1);
  Serial.print("address 1: ");
  Serial.print(addr_1);
  Serial.print("\t");
  Serial.print(value_1);
  Serial.println();
  delay(200);

  int value_2 = EEPROM.read(addr_2);
  Serial.print("address 2: ");
  Serial.print(addr_2);
  Serial.print("\t");
  Serial.print(value_2);
  Serial.println();
  delay(200);
  ///////////////////////////

  value_1++;
  value_2++;

  EEPROM.write(addr_1, value_1);
  EEPROM.commit();
  delay(200);

  EEPROM.write(addr_2, value_2);
  EEPROM.commit();
  delay(200);

  if (value_2 == check_value) {
    Serial.println("match value!");
  }

}
