/*  Simple (almost) VISA-compliant firmware for Arduino (Nano 33 IoT)

    This code implements SCPI-like commands. All commands only have one form;
    there is no distinction between full-length and abbreviated commands. Also,
    all commands should be send in uppercase without leading or trailing
    whitespace. Lastly, this code only accepts single commands. Multiple
    commands separated by semicolons are NOT supported.

    The firmware initializes both ADC and DAC to use 10-bit resolution and
    expects the operating voltage to be 3.3 V. For other boards than the Arduino
    Nano 33 IoT, change those values if necessary.

    Commands to the device should be terminated with a linefeed '\n'. Commands
    from the device are terminated with a carriage return and linefeed '\r\n'.

    Example usage:

    > *IDN?
    Arduino VISA firmware v0.2
    > OUT:CH0:VOLT 2.8
    2.80
    > MEAS:CH1?
    292
    > MEAS:CH1:VOLT?
    0.95
    > MEAS:CH2:VOLT?
    2.75

*/

#include <Regexp.h>

#define DAC_BITS      10
#define VOLTAGE       3.3

#define COM_IDN             "*IDN?"                       // literal *IDN?
#define COM_WRITE_DAC       "^OUT:CH(%d) (%d+)$"          // e.g. OUT:CH0 1023
#define COM_WRITE_DAC_VOLT  "^OUT:CH(%d):VOLT ([.%d]+)$"  // e.g. OUT:CH0:VOLT 2.5
#define COM_READ_DAC        "^OUT:CH(%d)%?$"              // e.g. OUT:CH0?
#define COM_READ_DAC_VOLT   "^OUT:CH(%d):VOLT%?$"         // e.g. OUT:CH0:VOLT?
#define COM_READ_ADC        "^MEAS:CH(%d)%?$"             // e.g. MEAS:CH1?
#define COM_READ_ADC_VOLT   "^MEAS:CH(%d):VOLT%?$"        // e.g. MEAS:CH1:VOLT?

#define IDN_STRING          "Arduino VISA firmware v0.2"
#define INVALID_CHANNEL_MSG "ERROR: Invalid channel number"

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

  // setup ADC and DAC channel(s)
  analogReadResolution(DAC_BITS);
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
  float volt;

  // read line from serial connection
  msg = Serial.readStringUntil('\n');
  // and save to buffer for regex matching
  msg.toCharArray(buffer, BUFFER_LENGTH);
  ms.Target(buffer);

  // request identification
  if (msg == COM_IDN) {
    Serial.println(IDN_STRING);
  }

  // write DAC value
  else if (ms.Match(COM_WRITE_DAC) == 1) {
    channel = atoi(ms.GetCapture(buffer, 0));
    if (channel >= 0 && channel < MAX_DAC_CHANNEL) {
      value = atoi(ms.GetCapture(buffer, 1));
      analogWrite(DACchannel[channel], value);
      DACvalues[channel] = value;
      Serial.println(value);
    }
    else Serial.println(INVALID_CHANNEL_MSG);
  }

  // write DAC value in volts
  else if (ms.Match(COM_WRITE_DAC_VOLT) == 1) {
    channel = atoi(ms.GetCapture(buffer, 0));
    if (channel >= 0 && channel < MAX_DAC_CHANNEL) {
      volt = atof(ms.GetCapture(buffer, 1));
      value = int(fmap(volt, 0, VOLTAGE, 0, 1023));
      analogWrite(DACchannel[channel], value);
      DACvalues[channel] = value;
      Serial.println(volt);
    }
    else Serial.println(INVALID_CHANNEL_MSG);
  }

  // request current DAC value
  else if (ms.Match(COM_READ_DAC) == 1) {
    channel = atoi(ms.GetCapture(buffer, 0));
    if (channel >= 0 && channel < MAX_DAC_CHANNEL) {
      Serial.println(DACvalues[channel]);
    }
    else Serial.println(INVALID_CHANNEL_MSG);
  }

  // request current DAC value in volts
  else if (ms.Match(COM_READ_DAC_VOLT) == 1) {
    channel = atoi(ms.GetCapture(buffer, 0));
    if (channel >= 0 && channel < MAX_DAC_CHANNEL) {
      value = DACvalues[channel];
      volt = fmap(value, 0, 1023, 0, VOLTAGE);
      Serial.println(volt);
    }
    else Serial.println(INVALID_CHANNEL_MSG);
  }

  // request ADC measurement value
  else if (ms.Match(COM_READ_ADC) == 1) {
    channel = atoi(ms.GetCapture(buffer, 0));
    if (channel >= 0 && channel < MAX_ADC_CHANNEL) {
      Serial.println(analogRead(ADCchannel[channel]));
    }
    else Serial.println(INVALID_CHANNEL_MSG);
  }

  // request ADC measurement value in volts
  else if (ms.Match(COM_READ_ADC_VOLT) == 1) {
    channel = atoi(ms.GetCapture(buffer, 0));
    if (channel >= 0 && channel < MAX_ADC_CHANNEL) {
      value = analogRead(ADCchannel[channel]);
      volt = fmap(value, 0, 1023, 0, VOLTAGE);
      Serial.println(volt);
    }
    else Serial.println(INVALID_CHANNEL_MSG);
  }

  // unknown command
  else {
    Serial.print("ERROR: UNKNOWN COMMAND ");
    Serial.println(msg);
  }
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  /* map()-replacement using floating point math */
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
