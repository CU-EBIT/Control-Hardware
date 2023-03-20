#pragma once
#include <Arduino.h>

void disableTXRXLEDs()
{
//  pinMode(PIN_LED_TXL, INPUT);   // turn off the USB activity
//  pinMode(PIN_LED_RXL, INPUT);   // LEDS to better see the yellow LED
}

unsigned long setupUSBSerial(int timout=10000){
  Serial.begin(1337);
  bool connected = (Serial); // Init USB serial, and wait for it to connect
  unsigned long t0 = millis();
  while (!connected && (millis() - t0 < timout)) connected = (Serial);
  return t0;
}
