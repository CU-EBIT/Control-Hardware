#pragma once
#ifndef SerialOut
#define SerialOut SerialUSB
#endif
#ifndef SerialIn
#define SerialIn SerialOut
#endif
#ifndef RATE
#define RATE 9600
#endif

struct data_10T {
  int id = SENS_ID;
  String key = String("ID:") + id;
  float T[10];

  void init(bool wait=false, bool usb_enabled=true, int timout=10000)
  {
    SerialOut.begin(RATE);
    SerialIn.begin(RATE);
    if (!wait) return;

    unsigned long t0 = millis();
    bool connected = (SerialOut);
    while (!connected && (millis() - t0 < timout)) connected = (SerialOut);
    SerialOut.printf("Ready, took %.2f s to initialise\r", (millis() - t0) / 1e3);

    t0 = millis();
    connected = (SerialIn);
    while (!connected && (millis() - t0 < timout)) connected = (SerialIn);
  }

  void print(bool check=true, bool json=true) {
    if(check and !shouldPrint()) return;
    if(json){
      SerialOut.print("{\"id\":");
      SerialOut.print(id);
    }
    for (int i = 0; i < 10; i++)
    {
      SerialOut.printf(",\"T_%d\":", i);
      SerialOut.print(T[i]);
    }
    if(json) SerialOut.print("}");
    SerialOut.print("\n");
  }

  bool shouldPrint()
  {
    if (SerialIn.available())
    {
      return SerialIn.readString().startsWith(key);
    }
    return false;
  }
};
