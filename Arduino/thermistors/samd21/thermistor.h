#pragma once
#define THERMISTOR_CACHE_SIZE 512 // Readings are an average of this many

class Thermistor{
public:
    Thermistor(int read_pin=0); // Initialise with a pin
    void setParams(float V0, float V1, float VMax, float T0, float T1); // Sets the parameters and pre-computes values
    void setPin(int pin); // Sets the pin we are reading from
    float getV(void);     // returns the averaged raw "voltage" reading from the pin
    float getT(void);     // returns the computed averaged temperature value
    void update(void);    // Reads from the ADC, and updates the sum and cache of readings
    void calculate(void); // Calculates temperature and average voltage
    
    float B_inv;           // We calculate this in setParams
    float V_max, V_0, V_1; // Max voltage, V at T_0, V at T_1
    float T_0, T_1;        // Reference Temperatures 0 and 1
    float T, v_avg;        // Output Temperature, Average Voltage
    int pin, v_sum;        // Pin number and cached sum of reads
    
    unsigned int n = 0;               // Number read
    int v_arr[THERMISTOR_CACHE_SIZE]; // Cache of reads
    float R_0, T_0_inv; // Caches used in computing values
};
