const char* start = "start";
const char* idnq = "*idn?";

void setup_serial(const char* idn){
  bool connected = (SerialUSB);
  // Start with a serial connection check loop, if we don't exit here
  // then we have no connection to get things from!
  while(!connected) connected = (SerialUSB);

  // buffer for reading values
  char buffer[10];
  int i = 0;
  // Now loop until we get a "start" message.
  
  while(1){
      while(SerialUSB.available()){
        char r = SerialUSB.read();
        if(r == '\r' or r == '\n'){
          //Clear rest of buffer.
          while(SerialUSB.available()) SerialUSB.read();
          bool isidn = true;
          bool done = true;
          for(int j = 0; j<i and (isidn or done); j++){
            isidn &= j<5 and buffer[j] == idnq[j];
            done &= j<5 and buffer[j] == start[j];
          }
          if(isidn){
            SerialUSB.println(idn);
          }
          else if(done){
            SerialUSB.println("Starting");
            return;
          }
          else{
            SerialUSB.println("Cmd Error!");
          }
          i = 0;
        }
        else{
          buffer[i++] = r;
          if(i>9) i = 0;
        }
    }
  }
}
