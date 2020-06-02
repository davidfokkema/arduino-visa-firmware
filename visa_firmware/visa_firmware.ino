void setup() {
  Serial.begin(9600);
  Serial.setTimeout(-1);
  Serial.flush();
}

void loop() {
  String msg;
  
  msg = Serial.readStringUntil('\n');
  Serial.print("I have received: ");
  Serial.println(msg);
}
