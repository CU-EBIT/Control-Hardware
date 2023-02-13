#include "magnets.h"
#include "xiao_utils.h"

// Our id for serial
#define SENS_ID 6

Magnetometer S0(0);
Magnetometer S1(1);
Magnetometer S2(2);
Magnetometer S3(3);

unsigned long tick;

void setup() {
  disableTXRXLEDs();
  setupUSBSerial();
}

void print()
{
  SerialUSB.print("{\"id\":");
  SerialUSB.print(SENS_ID);
  SerialUSB.print(",\"B0\":");
  SerialUSB.print(S0.B, 2);
  SerialUSB.print(",\"B1\":");
  SerialUSB.print(S1.B, 2);
  SerialUSB.print(",\"B2\":");
  SerialUSB.print(S2.B, 2);
  SerialUSB.print(",\"B3\":");
  SerialUSB.print(S3.B, 2);
  SerialUSB.println("}");
}

void loop() {
  S0.update();
  S1.update();
  S2.update();
  S3.update();
  // _CACHE_SIZE is defined in magnets.h
  if (!((tick++) % _CACHE_SIZE)) {
    print();
  }
}
