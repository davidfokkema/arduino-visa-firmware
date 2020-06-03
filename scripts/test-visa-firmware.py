import pyvisa
import time


rm = pyvisa.ResourceManager("@py")
dev_str = rm.list_resources()[-1]
dev = rm.open_resource(dev_str)
dev.write_termination = '\n'
dev.read_termination = '\r\n'


print(dev.query("*IDN?"))
print(dev.query("OUT:CH1 511"))
time.sleep(.5)
print(dev.query("OUT:CH1 767"))
time.sleep(.5)
print(dev.query("OUT:CH1 1023"))
time.sleep(1)
print(dev.query("OUT:CH1?"))
print(dev.query("OUT:CH1 0"))
print(dev.query("OUT:CH1?"))
