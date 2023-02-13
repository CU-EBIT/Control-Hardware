#pragma once
#define _CACHE_SIZE 1024

class Magnetometer{
public:
    Magnetometer(int read_pin, float B_0=0);
    void update(void);
//protected:
    float B = 0;
    float B_0 = 0;
    int pin, v_sum;
    unsigned long n = 0;
    int v_arr[_CACHE_SIZE];
};
