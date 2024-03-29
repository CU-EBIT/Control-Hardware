#define IDN "10_thermistor_reader"

// Defines for the serial comms
#define SerialOut SerialUSB
#define SerialIn SerialOut
#define USB_SERIAL true // If true, we can use pins 6 and 7
#define RATE 115200
// Our id for serial
#define SENS_ID 5
#define SENS_COUNT 10


#include "thermistor.h"
#include "xiao_utils.h"
#include "data_sending.h"
#include "rs232_init.h"

#define REPORT_MODE 0 // Modes: 0 -> temperatue, 1 -> voltage

#define T_LOW 273.15
#define T_HIGH 373.15

// These two are in order of number of the sensor, not index in array
const float V_0[SENS_COUNT] = {3112.86, 3112.86, 3112.86, 3112.86, 3112.86, 3112.86, 3112.86, 3112.86, 3112.86, 3112.86};
const float V_1[SENS_COUNT] = {271.36, 271.36, 271.36, 271.36, 271.36, 271.36, 271.36, 271.36, 271.36, 271.36};
const float V_m[SENS_COUNT] = {4086.39, 4086.39, 4086.39, 4086.39, 4086.39, 4086.39, 4086.39, 4086.39, 4086.39, 4086.39};

// Pin to sensor mapping
const int ind[SENS_COUNT] = {8, 9, 4, 5, 6, 1, 0, 7, 2, 3};

Thermistor T[SENS_COUNT]; // thermistor objects to manage
data_10T Data;            // Data handler for sending the values on serial

unsigned long tick = 0; // timer used for update order

void setup()
{
  disableTXRXLEDs();
  setupUSBSerial();
  Data.init();
  // Initialise the values for the thermistors
  for(int i = 0; i<SENS_COUNT; i++){
    int s = ind[i]; // This gets us the pin->named sensor map
    T[s].setPin(i+A0);
    T[s].setParams(V_0[s] ,V_1[s], V_m[s], T_LOW, T_HIGH);
  }
}

// IO buffer stuff for command handling
#define BUF_LEN 32 // We only listen for commands *idn? and read?, so 32 chars is plenty
#define BUF_END (BUF_LEN - 1)
#define BUF_MID (BUF_LEN / 2)
int buf_i = BUF_MID;// Read buffer index
char buf[BUF_LEN];  // Read buffer
void read_from_usb(){
    while (SerialIn.available()) { // If anything comes in Serial (USB),
    // buffer overflow?           // check if it is idn?, otherwise ignore it
    if(buf_i > BUF_END){
      memcpy(buf, buf+BUF_MID, BUF_MID);
      buf_i = BUF_MID;
    }
    char r = SerialIn.read();
    if (not USB_SERIAL and SerialUSB) SerialUSB.print(r);
    buf[buf_i] = r;
    if(r == '\r' or r == '\n') {
      if(check_idn(buf + buf_i, buf_i));
      else if(check_read(buf + buf_i, buf_i)){
        for(int i = 0; i<SENS_COUNT; i++){
          // If not USB_SERIAL, then we need to
          // not read on A6 or A7 for the seeeduino
          // If not using a seeeduino, change these accordingly.
#if not USB_SERIAL
          int pin = T[i].pin;
          if(pin == A6 or pin == A7){ continue; }
#endif
          T[i].calculate(); // Do the expensive floating point maths here

          // Now determine what to report based on the REPORT_MODE
#if REPORT_MODE == 0
          Data.T[i] = T[i].getT();
#elif REPORT_MODE == 1
          Data.T[i] = T[i].getV();
#endif
        }

        // Finally print all values out, as json, and ignoring if we are told and ID
        Data.print(false);
      }
    }
    buf_i++;
  }
}

void loop()
{
  // Read in the Serial commands
  read_from_usb();
  // Now tick our sensors
  auto* _T = &T[tick%10];
#if not USB_SERIAL
  int pin = _T->pin;
  // If we are not usb connected, these pins are reserved
  // If not using a seeeduino, change these accordingly.
  if(pin == A6 or pin == A7){ goto _end; }
#endif
  _T->update();
_end:
  tick++;
}
