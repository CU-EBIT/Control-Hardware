#pragma once
#define THERMISTOR_CACHE_SIZE 512

class Thermistor{
public:
    Thermistor(int read_pin=0, float R0=10000, float R1=10000);
    void setB(float B);
    void setR0(float R0);
    void setR1(float R1);
    void setT0(float T0);
    void setPin(float pin);
    float getR(void);
    float getV(void);
    float getT(void);
    void update(void);
//protected:
    float B = 3950;
    float R0 = 10000;
    float R1 = 10000;
    int pin, v_sum;
    float T0 = (273.15 + 25);
    float logR2, R2, T, v_avg;
    unsigned long n = 0;
    int v_arr[THERMISTOR_CACHE_SIZE];
};

float calculateB(float& logR2, float& T, float& T0);
void calculateR2(int& value, float& R0, float& R1, float& R2, float& logR2, float& v_avg);
