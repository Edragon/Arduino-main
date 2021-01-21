#define relay      13

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  Serial.println("begin..");
  digitalWrite(relay, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

}
