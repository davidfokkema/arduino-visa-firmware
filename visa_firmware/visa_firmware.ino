#define COMM_IDN    "*IDN?"
#define IDN_STRING  "Arduino VISA firmware v0.1"

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(-1);
  Serial.flush();
}

void loop() {
  String msg;
  
  msg = Serial.readStringUntil('\n');

  if (msg == COMM_IDN) {
    Serial.println(IDN_STRING);
  }
  else {
    Serial.print("I have received: ");
    Serial.println(msg);
  }
}
