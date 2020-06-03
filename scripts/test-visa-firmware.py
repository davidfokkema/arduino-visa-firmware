import pyvisa

rm = pyvisa.ResourceManager("@py")
dev_str = rm.list_resources()[-1]
dev = rm.open_resource(dev_str)
dev.write_termination = '\n'
dev.read_termination = '\r\n'

print(dev.query("*IDN?"))
