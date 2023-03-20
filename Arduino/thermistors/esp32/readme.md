# How to calibrate

The thermistor reader assumes a mostly stable reference resistor is used, where the thermistor and reference resistor form a voltage divider, from V_max to GND, with the reference on the high side of the divider.

The calibration is then 3 voltages, which allow internally computing the reference resistor value, as well as the B parameter for the thermistor, so this requires 3 separate logs of ADC readings, the example here has them named `273.15.vals`, `373.15.vals` and `MAX.vals`. The thermistor is assumed to be able to be un-plugged from the board, to be able to read the high value for the rail.

## Required materials

1. Cold temperature reference, ice bath works well
2. Hot temperature reference, kettle works well

## Calibration steps

1. Upload Thermistor_Calibrate the the micro-controller, and set `CAL_PIN` according to the thermistor in question.
2. Connect the thermistor the the appropriate pins, and place it in the Cold reference, wait until it equilibriates, this can be checked by running the Serial Plotter in the Arduino IDE, and waiting for it to stop trending upwards.
3. Stop running Serial Plotter to free up the port, and run `calibrate.py`, note it will log the files based on the `T` variable, so change that properly if you want to save the values.
4. `calibrate.py` prints 3 values to terminal, average, standard deviation, and relative standard deviation. You only need average for this, copy that value and store it somewhere as the V_0.
5. Repeat steps 2-4, except in the Hot reference, and store that value as V_1.
6. Unplug the thermistor, and repeat steps 2-4, storing the value as V_m.
7. Enter V_0, V_1 and V_m into the appropriate array locations in `thermistor.ino`.