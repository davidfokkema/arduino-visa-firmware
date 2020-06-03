#include <Regexp.h>

#define DAC_BITS      10

#define COM_IDN       "*IDN?"
#define COM_WRITE_DAC "^OUT:CH(%d) (%d+)$"
#define COM_READ_DAC  "^OUT:CH(%d)?$"

#define IDN_STRING    "Arduino VISA firmware v0.1"

#define BUFFER_LENGTH 100

int DACchannel[] = {DAC0};
int ADCchannel[] = {A0, A1, A2, A3, A4, A5, A6, A7};

#define MAX_DAC_CHANNEL sizeof(DACchannel)  / sizeof(int)
#define MAX_ADC_CHANNEL sizeof(ADCchannel)  / sizeof(int)

int DACvalues[MAX_DAC_CHANNEL];

void setup() {
  int i;
  
  Serial.begin(9600);
  Serial.setTimeout(-1);
  Serial.flush();

  analogWriteResolution(DAC_BITS);
  for (i = 0; i < MAX_DAC_CHANNEL; i ++) {
    analogWrite(DACchannel[i], 0);
    DACvalues[i] = 0;
  }
}

void loop() {
  String msg;
  MatchState ms;
  char buffer[BUFFER_LENGTH];
  int channel, value;
  
  msg = Serial.readStringUntil('\n');
  msg.toCharArray(buffer, BUFFER_LENGTH);
  ms.Target(buffer);

  if (msg == COM_IDN) {
    Serial.println(IDN_STRING);
  }
  else if (ms.Match(COM_WRITE_DAC) == 1) {
    channel = atoi(ms.GetCapture(buffer, 0));
    value = atoi(ms.GetCapture(buffer, 1));
    analogWrite(DACchannel[channel - 1], value);
    DACvalues[channel - 1] = value;
    Serial.println(value);
  }
  else if (ms.Match(COM_READ_DAC) == 1) {
    channel = atoi(ms.GetCapture(buffer, 0));
    Serial.println(DACvalues[channel - 1]);
  }
  else {
    Serial.print("UNKNOWN COMMAND: ");
    Serial.println(msg);
  }
}
