#define IDN "data_tx"
#include "rs232_init.h"
#include "xiao_utils.h"

#define SerialOut Serial1

int len = 0;
bool started;

void setup() {
  disableTXRXLEDs();
  setupUSBSerial();
  SerialOut.begin(57600);
}

bool state = HIGH;

#define BUF_LEN 2048
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
      goto loop_end;
    }
    if(started){
      if(len-- <= 0){
        state = !state;
        for(int j = BUF_MID - 6; j <= buf_i; j++){
          SerialUSB.print(buf[j]);
          SerialOut.write(buf[j]);
        }
        SerialUSB.print('\n');
        
        started = false;
        continue;
      }
    }
    else {
      started = buf[buf_i-5] == 't' and buf[buf_i-4] == 'x' and buf[buf_i-3] == ':' and buf[buf_i-2] == ' ';
      if(started) {
        // update length
        len = buf[buf_i-1] + (buf[buf_i-0] << 5)-1;

        // Stuff these in here for the message
        for(int i = -5; i<=0; i++) buf[BUF_MID + (i-1)] = buf[buf_i + i];
        
        // no need to re-roll the buffer in this case.
        buf_i = BUF_MID;
        continue;
      }
    }
loop_end:
    buf_i++;
  }
}

void loop() {
  read_from_usb();
  digitalWrite(13, state);
  while (SerialOut.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    SerialOut.read();                 // flush Serial1's read buffer, we don't care about it
  }
}
