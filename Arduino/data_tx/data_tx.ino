#include "rs232_init.h"

void setup() {
  SerialUSB.begin(57600);
  Serial1.begin(57600);
  setup_serial("data_tx");
}

void loop() {
  if (SerialUSB.available()) {      // If anything comes in Serial (USB),
    Serial1.write(SerialUSB.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial1.read();   // flush Serial1's read buffer, we don't care about it
  }
}
