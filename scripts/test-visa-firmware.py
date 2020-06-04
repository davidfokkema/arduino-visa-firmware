import pyvisa
import time


rm = pyvisa.ResourceManager("@py")
dev_str = rm.list_resources()[-1]
dev = rm.open_resource(dev_str)
dev.write_termination = '\n'
dev.read_termination = '\r\n'


print(dev.query("*IDN?"))
print(dev.query("OUT:CH1 1023"))
print(dev.query("OUT:CH1?"))
time.sleep(.5)

for i in range(0, 1023):
    dev.query(f"OUT:CH1 {i}")
    print(i, dev.query("MEAS:CH2:VAL?"))
