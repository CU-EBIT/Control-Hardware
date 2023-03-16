#define IDN "10_thermistor_reader"

// Defines for the serial comms
#define SerialOut Serial1
#define SerialIn SerialOut
#define USB_SERIAL false // If true, we can use pins 6 and 7
#define RATE 115200
// Our id for serial
#define SENS_ID 5


#include "thermistor.h"
#include "xiao_utils.h"
#include "data_sending.h"
#include "rs232_init.h"

#define REPORT_MODE 0 // Modes: 0 -> temperatue, 1 -> resistance, 2 -> voltage

// These two are in order of number of the sensor, not index in array
const float R_0[10] = {31167, 31243, 31390, 31275, 31473, 31682, 31194, 31346, 31390, 31359};
const float B[10] =   {3868,  3867,  3893,  3871,  3882,  3830,  3871,  3881,  3890,  3896};

// These two are for converting from pin to sensor, and the resistors are in order of pin
const float R_1[10] = {10072, 10021, 10027, 9984.7, 9988.5, 10010, 10029, 9985.1, 10027,10000};
// Pin to sensor mapping
const int ind[10] = {8, 9, 4, 5, 6, 1, 0, 7, 2, 3};

Thermistor T[10];
data_10T Data;

int n = 0;

unsigned long tick = 0;

void setup()
{
  disableTXRXLEDs();
  setupUSBSerial();
  Data.init();

  for(int i = 0; i<10; i++){
    int s = ind[i];
    T[s].setPin(i+1);
    T[s].setR0(R_0[s]);
    T[s].setR1(R_1[i]);
    T[s].setT0(273.15);
    T[s].setB(B[s]);
  }
}

#define BUF_LEN 32
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
    if (SerialUSB) SerialUSB.print(r);
    buf[buf_i] = r;
    if(r == '\r' or r == '\n') {
      if(check_idn(buf + buf_i, buf_i));
      else if(check_read(buf + buf_i, buf_i)){
        Data.print(false);
      }
    }
    buf_i++;
  }
}

void loop()
{
  read_from_usb();

  T[tick%10].update();

#if REPORT_MODE == 0
  Data.T[tick%10] = T[tick%10].getT();
#elif REPORT_MODE == 1
  Data.T[tick%10] = T[tick%10].getR();
#elif REPORT_MODE == 2
  Data.T[tick%10] = T[tick%10].getV();
#endif
  
  tick++;
}
