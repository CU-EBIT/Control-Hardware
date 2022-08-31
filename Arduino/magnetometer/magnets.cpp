#include "magnets.h"
#include <Arduino.h>

Magnetometer::Magnetometer(int read_pin, float B_0)
{
  this->B_0 = B_0;
  this->pin = read_pin;
  pinMode(read_pin, INPUT);
  analogReadResolution(12);
  this->v_sum = 0;
}

void Magnetometer::update(void)
{
  int value = analogRead(pin);
  float v = value;
  v_arr[n % _CACHE_SIZE] = value;
  if (n++ > _CACHE_SIZE)
  {
    if (!v_sum)
    {
      for (int i = 0; i < _CACHE_SIZE; i++) v_sum += v_arr[i];
    }
    else
    {
      v_sum += value;
      v_sum -= v_arr[n % _CACHE_SIZE];
      v = v_sum;
    }
  }
  else v *= _CACHE_SIZE;
  B = v / _CACHE_SIZE - B_0;
}
