#include "thermistor.h"
#include <Arduino.h>

Thermistor::Thermistor(int read_pin)
{
  // Set the pin, this also sets pinmode
  this->setPin(read_pin);
  // Set to 12 bit read resolution
  analogReadResolution(12);
  analogSetAttenuation(ADC_6db);
  // Initialise sum and cache array to 0
  this->v_sum = 0;
  memset(v_arr, 0, THERMISTOR_CACHE_SIZE);
}

float Thermistor::getT(void)
{
  return T;
}

float Thermistor::getV(void)
{
  return v_avg;
}

void Thermistor::setPin(int pin)
{
  this->pin = pin;
  // Also mark pin as input if needed
  pinMode(pin, INPUT);
}

void Thermistor::setParams(float V0, float V1, float VMax, float T0, float T1)
{
  T_0 = T0;
  T_1 = T1;
  V_0 = V0;
  V_1 = V1;
  V_max = VMax;
  
  float R_1, T_1_inv, T_diff_inv; // We need these two for B, but not later
  R_0 = 1.0 / (V_max / V_0 - 1.0); // These result in "resistances" in reference to V_max and an unknown reference
  R_1 = 1.0 / (V_max / V_1 - 1.0);
  T_0_inv  = 1.0 / T_0;
  T_1_inv  = 1.0 / T_1;
  T_diff_inv = (T_1_inv - T_0_inv);
  B_inv = (1.0 / log(R_1/R_0)) * T_diff_inv;
}

void Thermistor::calculate(void)
{
  float v = v_sum;
  float denom = n > THERMISTOR_CACHE_SIZE ? THERMISTOR_CACHE_SIZE : n;
  v_avg = v / THERMISTOR_CACHE_SIZE;
  float R = 1.0 / (V_max / v_avg - 1.0);
  T = 1.0 / (log(R / R_0)*B_inv + T_0_inv);
}

void Thermistor::update(void)
{
  int value = analogReadMilliVolts(pin);

  // We place the current value in the cache, and then pop the next value from the cache.
  // If cache isn't full, the popped value was set as 0 anyway, so nothing happens.
  // this results in v_sum being a rolling sum of the last THERMISTOR_CACHE_SIZE values read
  
  this->v_arr[this->n++ % THERMISTOR_CACHE_SIZE] = value;
  this->v_sum += value;
  this->v_sum -= this->v_arr[this->n % THERMISTOR_CACHE_SIZE];
  
  // Overflow protection
  if(this->n > 2 * THERMISTOR_CACHE_SIZE) this->n -= THERMISTOR_CACHE_SIZE;
}
