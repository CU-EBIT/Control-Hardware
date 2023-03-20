import serial
import pyqtgraph as pg
import numpy as np
import time

T = "MAX"
file = f'./{T}.vals'

vars = []
with serial.Serial("COM22", 115200) as ser:
    file = open(file, 'w')
    for i in range(1024):
        var = ser.readline().decode().strip()
        vars.append(float(var))
        file.write(f'{var}\n')
        if i%32==0:
            print(".", end='',flush=True)
            time.sleep(0.001)
    file.close()

vars = np.array(vars)
avg = np.sum(vars) / len(vars)
std = np.std(vars)
std_avg = std / avg
print("")
print(avg, std, std_avg)

pg.plot(vars)
pg.exec()
