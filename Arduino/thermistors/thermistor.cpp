#include "thermistor.h"
#include <Arduino.h>

Thermistor::Thermistor(int read_pin, float R0, float R1)
{
  this->setR0(R0);
  this->setR1(R1);
  this->pin = read_pin;
  pinMode(read_pin, INPUT);
  analogReadResolution(12);
  this->v_sum = 0;
}

float Thermistor::getR(void)
{
  return R2;
}

float Thermistor::getT(void)
{
  return T;
}

float Thermistor::getV(void)
{
  return v_avg;
}

void Thermistor::setPin(float pin)
{
  this->pin = pin;
}

void Thermistor::setT0(float T0)
{
  this->T0 = T0;
}

void Thermistor::setR0(float R0)
{
  this->R0 = R0;
}

void Thermistor::setR1(float R1)
{
  this->R1 = R1;
}

void Thermistor::setB(float B)
{
  this->B = B;
}

void calculateR2(float& v, float& R0, float& R1, float& R2, float& logR2, float& v_avg)
{
  v_avg = v / THERMISTOR_CACHE_SIZE;
  R2 = (4095.0 / v_avg) - 1;
  R2 = R1 / R2;

  logR2 = R2 / R0;
  logR2 = log(logR2);
}

float calculateB(float& logR2, float& T, float& T0)
{
  return logR2 / (1 / T - 1 / T0);
}

void Thermistor::update(void)
{
  int value = analogRead(pin);
  float v = value;
  v_arr[n % THERMISTOR_CACHE_SIZE] = value;
  if (n++ > THERMISTOR_CACHE_SIZE)
  {
    if (!v_sum)
    {
      for (int i = 0; i < THERMISTOR_CACHE_SIZE; i++) v_sum += v_arr[i];
    }
    else
    {
      v_sum += value;
      v_sum -= v_arr[n % THERMISTOR_CACHE_SIZE];
      v = v_sum;
    }
  }
  else v *= THERMISTOR_CACHE_SIZE;

  calculateR2(v, R0, R1, R2, logR2, v_avg);
  
  T = 1 / (logR2 / B + 1 / T0);
}
