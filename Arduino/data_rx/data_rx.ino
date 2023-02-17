#define IDN "data_rx"
#include "rs232_init.h"
#include "xiao_utils.h"

bool state = HIGH;

void setup() {
  disableTXRXLEDs();
  setupUSBSerial();
  Serial1.begin(57600);
}

#define BUF_LEN 32
#define BUF_END (BUF_LEN - 1)
#define BUF_MID (BUF_LEN / 2)
int buf_i = BUF_MID;// Read buffer index
char buf[BUF_LEN];  // Read buffer
void read_from_usb(){
    while (SerialUSB.available()) { // If anything comes in Serial (USB),
    // buffer overflow?           // check if it is idn?, otherwise ignore it
    if(buf_i > BUF_END){
      memcpy(buf, buf+BUF_MID, BUF_MID);
      buf_i = BUF_MID;
    }
    char r = SerialUSB.read();
    buf[buf_i] = r;
    if(r == '\r' or r == '\n') {
      check_idn(buf + buf_i, buf_i);
    }
    buf_i++;
  }
}

void loop() {
  read_from_usb();
  digitalWrite(13, state);
  while (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    state = !state;
    SerialUSB.write(Serial1.read());   // read it and send it out Serial (USB)
  }
}
