#ifndef SerialOut
#define SerialOut SerialUSB
#endif

const char* readq = "read?";
const char* idnq = "*idn?";

bool check_idn(char*buf, int len){
    if(len < 5) return false;
    bool isidn = true;
    for(int j = -5; j<0 and isidn; j++){
      isidn &= buf[j] == idnq[j + 5];
    }
    if(isidn){ 
      SerialOut.print(IDN);
      SerialOut.print('\n');
    }
    return isidn;
}

bool check_read(char*buf, int len){
    if(len < 5) return false;
    bool isidn = true;
    for(int j = -5; j<0 and isidn; j++){
      isidn &= buf[j] == readq[j + 5];
    }
    return isidn;
}
