#define IDN "data_rx"
#include "rs232_init.h"

int i = 0;
char buf_r[10]; // Read buffer

void setup() {
  Serial.begin(57600);
  Serial1.begin(57600);
}
bool state = HIGH;

void loop() {
  digitalWrite(13, state);
  while (SerialUSB.available()) { // If anything comes in Serial (USB),
    // buffer overflow?           // check if it is idn?, otherwise ignore it
    if(i > 9) i = 0;
    char r = SerialUSB.read();
    buf_r[i] = r;
    if(r == '\r' or r == '\n' and i >= 5) {
      check_idn(buf_r);
      i = 0;
      continue;
    }
    i++;
  }

  while (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    state = !state;
    SerialUSB.write(Serial1.read());   // read it and send it out Serial (USB)
  }
}
