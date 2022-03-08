#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h" 

int getsw(void){
int switches = (PORTD>>8) & 0x000f;
    return switches;
   
}

int getbtns(void){
    int buttons = (PORTD>>5) & 0x0007;
      return buttons;
}