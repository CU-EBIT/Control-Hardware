#pragma once
#include <Arduino.h>

void disableTXRXLEDs()
{
  pinMode(PIN_LED_TXL, INPUT);   // turn off the USB activity
  pinMode(PIN_LED_RXL, INPUT);   // LEDS to better see the yellow LED
}

unsigned long setupUSBSerial(int timout=10000){
  SerialUSB.begin(1337);
  bool connected = (SerialUSB);
  unsigned long t0 = millis();
  while (!connected && (millis() - t0 < timout)) connected = (SerialUSB);
  SerialUSB.printf("Ready, took %.2f s to initialise\n", (millis() - t0) / 1e3);
  return t0;
}
