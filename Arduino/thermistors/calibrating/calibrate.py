import serial
import pyqtgraph as pg
import numpy as np

T = "MAX"
file = f'./{T}.vals'

vars = []
with serial.Serial("COM6", 115200) as ser:
    file = open(file, 'w')
    for _ in range(1024):
        var = ser.readline().decode().strip()
        vars.append(float(var))
        file.write(f'{var}\n')
    file.close()

vars = np.array(vars)
avg = np.sum(vars) / len(vars)
std = np.std(vars)
std_avg = std / avg
print(avg, std, std_avg)

pg.plot(vars)
pg.exec()
