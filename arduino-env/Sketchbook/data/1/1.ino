signed int answer;

// the setup function runs once when you press reset or power the board
void setup() {
  answer = strtol("F4AC", NULL, 16);
  Serial.begin(9600);   
}

// the loop function runs over and over again forever
void loop() {
  Serial.println(answer);
  
}
