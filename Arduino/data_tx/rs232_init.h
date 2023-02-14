const char* start = "start";
const char* idnq = "*idn?";

bool check_idn(char*buf){
    bool isidn = true;
    for(int j = 0; j<5 and isidn; j++){
      isidn &= j<5 and buf[j] == idnq[j];
    }
    if(isidn){ 
      SerialUSB.print(IDN);
      SerialUSB.print('\n');
    }
    return isidn;
}
