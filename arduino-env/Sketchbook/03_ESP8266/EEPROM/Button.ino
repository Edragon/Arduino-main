long Press_1 = 0;
long Press_2 = 10;
bool Press_flag = true;


void runRST () {
  Serial.print("press 1             ");
  Serial.println(Press_1);
  Serial.print("press 2            ");
  Serial.println(Press_2);
  if (Press_2 > Press_1)  {
    //Serial.print("press2 larger");
    if (Press_2 - Press_1 < 1000) {
      Serial.println("detect quick press............... type 2-1");
    }
  }
  if (Press_1 > Press_2) {
    //Serial.print("press1 larger");
    if (Press_1 - Press_2 < 1000) {
      Serial.println("detect quick press............... type 1-2");
    }
  }
}

void IRQ() {
  if (Press_flag) {
    Press_flag = !Press_flag;
    Press_1 = millis();
    runRST();
  }
  else {
    Press_flag = !Press_flag;
    Press_2 = millis();
    runRST();

  }
}


void btnIRQ() {
  attachInterrupt(btn, IRQ, FALLING);
}
