#include <p32xxxx.h>
#include <inttypes.h>
#include <LED.h>
#include <GPIODrv.h>


void LED_init(){
  
}

void LED_1_setState(uint8_t state){
  if (state){
    LED_1_LATbits.LED_1_LATPIN = 1;
  }
  else{
    LED_1_LATbits.LED_1_LATPIN = 0;
  }
}

void LED_2_setState(uint8_t state){
  if (state){
    LED_2_LATbits.LED_2_LATPIN = 1;
  }
  else{
    LED_2_LATbits.LED_2_LATPIN = 0;
  }
}


