#define IDN "data_tx"
#include "rs232_init.h"

#define SerialOut Serial1

int i = 0;
int len = 0;
bool started;
bool done;
char outer[1032];
char*buf_r = outer + 8; // Read buffer
char*buf_w = buf_r + 5; // Write buffer

void setup() {
  SerialUSB.begin(57600);
  SerialOut.begin(57600);
  memset(outer, 0, 1032);
}

bool state = HIGH;

void loop() {
  digitalWrite(13, state);
  while (SerialUSB.available()) {      // If anything comes in Serial (USB),
    // buffer overflow?
    if(i > 1023){ 
      i = 0;
      SerialUSB.print("overflow\n");
    }
    char r = SerialUSB.read();
    buf_r[i] = r;
    if((r == '\r' or r == '\n') and check_idn(buf_r + i - 5)) {
      i = 0;
      started = false;
      len = 0;
      continue;
    }
    if(started) {
      done = i > len - 2;
      SerialOut.write(r);
      if(done){
        state = !state;
        for(int j = 0; j <= i; j++){
          SerialUSB.print(buf_r[j]);
        }
        SerialUSB.print('\n');
        started = false;
        len = 0;
        i = 0;
        continue;
      }
      i++;
      continue;
    }
    if(not started) {
      started = buf_r[i-5] == 't' and buf_r[i-4] == 'x' and buf_r[i-3] == ':' and buf_r[i-2] == ' ';
      if(started) {
        // send out the number expected first.
        SerialOut.write(buf_r[i-1]);
        SerialOut.write(buf_r[i-0]);
        // Then update length and reset index.
        len = buf_r[i-1] + (buf_r[i-0] << 5);
        i = 0;
        continue;
      }
      if(i > 10) {
        i = 0;
        continue;
      }
    }
    i++;
  }
  while (SerialOut.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    SerialOut.read();                 // flush Serial1's read buffer, we don't care about it
  }
}
