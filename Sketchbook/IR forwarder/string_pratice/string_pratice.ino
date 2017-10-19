void setup() {
  Serial.begin(9600);
  String req = "http://192.168.0.103/rgb/00ff00";
  int pos = req.length(); // total lengh
  int ind1 = req.indexOf("/rgb/") + 5;  
  String teststring = req.substring(ind1, pos);
  Serial.println (teststring);
}

void loop() {
  // put your main code here, to run repeatedly:

}
