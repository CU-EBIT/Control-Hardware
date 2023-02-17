#pragma once

const char* idnq = "*idn?";

bool check_idn(char*buf, int len){
    if(len < 5) return false;
    bool isidn = true;
    for(int j = -5; j<0 and isidn; j++){
      isidn &= buf[j] == idnq[j + 5];
    }
    if(isidn){ 
      SerialUSB.print(IDN);
      SerialUSB.print('\n');
    }
    return isidn;
}
