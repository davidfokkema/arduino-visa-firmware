# arduino-visa-firmware

Simple (almost) VISA-compliant firmware for Arduino (Nano 33 IoT).

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
```
# Ask for identification
> *IDN?
Arduino VISA firmware v0.2

# Set the first analog output channel to output 2.8 V
> OUT:CH0:VOLT 2.8
2.80

# Measure the signal on the second analog input channel (raw value)
> MEAS:CH1?
292

# Measure the signal on the second analog input channel (in volts)
> MEAS:CH1:VOLT?
0.95

# Measure the signal on the third analog input channel (in volts)
> MEAS:CH2:VOLT?
2.75
```
