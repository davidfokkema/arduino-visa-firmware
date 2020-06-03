#include <Regexp.h>

#define COMM_IDN    "*IDN?"
#define COMM_DAC    "OUT:CH"

#define IDN_STRING  "Arduino VISA firmware v0.1"

#define BUFFER_LENGTH  100

int DACchannel[] = {DAC0};
int ADCchannel[] = {A0, A1, A2, A3, A4, A5, A6, A7};

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(-1);
  Serial.flush();
}

void loop() {
  String msg;
  MatchState ms;
  char buffer[BUFFER_LENGTH];
  int channel, value;
  
  msg = Serial.readStringUntil('\n');
  msg.toCharArray(buffer, BUFFER_LENGTH);
  ms.Target(buffer);

  if (msg == COMM_IDN) {
    Serial.println(IDN_STRING);
  }
  else if (ms.Match("OUT:CH(%d) (%d+)") == 1) {
    channel = atoi(ms.GetCapture(buffer, 0));
    value = atoi(ms.GetCapture(buffer, 1));
    Serial.print("DAC channel ");
    Serial.print(channel);
    Serial.print(" value ");
    Serial.println(value);
  }
//  else if (msg.startsWith(COMM_DAC)) {
//    channel = String(msg[sizeof(COMM_DAC) - 1]).toInt();
////    println(analogWrite(DACchannel[channel]));
//  }
  else {
    Serial.print("I have received: ");
    Serial.println(msg);
  }
}
