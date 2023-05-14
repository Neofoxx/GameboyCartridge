#include <p32xxxx.h>
#include <GPIODrv.h>
#include <inttypes.h>
#include <forNewlib.h>

void GPIODrv_init(){
  // Setup initial pin states
  LED_1_ANSEL_REG.LED_1_ANSEL_BIT = 0;  // Turn analog inputs into digital IO
  LED_1_TRISbits.LED_1_TRISPIN = 0;       // Set to output
  LED_1_LATbits.LED_1_LATPIN = 1;       // Show activity
  
  LED_2_ANSEL_REG.LED_2_ANSEL_BIT = 0;  // Turn analog inputs into digital IO
  LED_2_TRISbits.LED_2_TRISPIN = 0;      // Set to output
  LED_2_LATbits.LED_2_LATPIN = 1;       // Show activity
  
  WR_ANSEL_REG.WR_ANSEL_BIT = 0;  // Turn analog inputs into digital IO
  WR_TRISbits.WR_TRISPIN = 0;     // Set to output
  WR_LATbits.WR_LATPIN = 0;       // Deassert
  
  RD_ANSEL_REG.RD_ANSEL_BIT = 0;  // Turn analog inputs into digital IO
  RD_TRISbits.RD_TRISPIN = 0;     // Set to output
  RD_LATbits.RD_LATPIN = 0;       // Deassert
  
  GB_RESET_TRISbits.GB_RESET_TRISPIN = 0;     // Set to output
  GB_RESET_LATbits.GB_RESET_LATPIN = 1;       // Keep asserted
  
  EXT_1_TRISbits.EXT_1_TRISPIN = 1;     // Set to input
  EXT_2_TRISbits.EXT_2_TRISPIN = 1;     // Set to input
  EXT_2_TRISbits.EXT_2_TRISPIN = 1;     // Set to input
  
  SHIFT_LOAD_ANSEL_REG.SHIFT_LOAD_ANSEL_BIT = 0;  // Turn analog inputs into digital IO
  SHIFT_LOAD_TRISbits.SHIFT_LOAD_TRISPIN = 0;     // Set to output
  SHIFT_LOAD_LATbits.SHIFT_LOAD_LATPIN = 1;       // SHIFT == 1, 0 == LOAD
  
  SHIFT_CLK_ANSEL_REG.SHIFT_CLK_ANSEL_BIT = 0;  // Turn analog inputs into digital IO
  SHIFT_CLK_TRISbits.SHIFT_CLK_TRISPIN = 0;     // Set to output
  SHIFT_CLK_LATbits.SHIFT_CLK_LATPIN = 0;       // Deassert CLK
 
  SHIFT_OUT_ANSEL_REG.SHIFT_OUT_ANSEL_BIT = 0;  // Turn analog inputs into digital IO
  SHIFT_OUT_TRISbits.SHIFT_OUT_TRISPIN = 1;   // Set to input
  
  nADDR_OE_TRISbits.nADDR_OE_TRISPIN = 0;   // Set to output
  nADDR_OE_LATbits.nADDR_OE_LATPIN = 1;     // Deassert
  
  nDATA_OE_TRISbits.nDATA_OE_TRISPIN = 0;   // Set to output
  nDATA_OE_LATbits.nDATA_OE_LATPIN = 1;     // Deassert
  
  REG_CLK_TRISbits.REG_CLK_TRISPIN = 0;     // Set to output
  REG_CLK_LATbits.REG_CLK_LATPIN = 0;       // Deassert
  
  SHIFT_IN_TRISbits.SHIFT_IN_TRISPIN = 0;   // Set to output
  SHIFT_IN_LATbits.SHIFT_IN_LATPIN = 0;    // DNC
  
  DATA_CLK_TRISbits.DATA_CLK_TRISPIN = 0;   // Set to output
  DATA_CLK_LATbits.DATA_CLK_LATPIN = 0;     // TODO check later
 
}

void GPIODrv_setRD(uint8_t state){
  if (state){
    RD_LATbits.RD_LATPIN = 1;
  }
  else{
    RD_LATbits.RD_LATPIN = 0;
  }
}

void GPIODrv_setWR(uint8_t state){
  if (state){
    WR_LATbits.WR_LATPIN = 1;
  }
  else{
    WR_LATbits.WR_LATPIN = 0;
  }
}

void GPIODrv_setRST(uint8_t state){
  if (state){
    GB_RESET_LATbits.GB_RESET_LATPIN = 1;
  }
  else{
    GB_RESET_LATbits.GB_RESET_LATPIN = 0;
  }
}

// 595 shift register - SHIFT_IN, DATA_CLK, REG_CLK, ADDR_OE, DATA_OE
void GPIODrv_setSHIFTIN(uint8_t state){
  if (state){
    SHIFT_IN_LATbits.SHIFT_IN_LATPIN = 1;
  }
  else{
    SHIFT_IN_LATbits.SHIFT_IN_LATPIN = 0;
  }
}

void GPIODrv_setDATACLK(uint8_t state){
  if (state){
    DATA_CLK_LATbits.DATA_CLK_LATPIN = 1;
  }
  else{
    DATA_CLK_LATbits.DATA_CLK_LATPIN = 0;
  }
}

void GPIODrv_setREGCLK(uint8_t state){
  if (state){
    REG_CLK_LATbits.REG_CLK_LATPIN = 1;
  }
  else{
    REG_CLK_LATbits.REG_CLK_LATPIN = 0;
  }
}

void GPIODrv_setADDROE(uint8_t state){
  if (state){
    nADDR_OE_LATbits.nADDR_OE_LATPIN = 1;
  }
  else{
    nADDR_OE_LATbits.nADDR_OE_LATPIN = 0;
  }
}

void GPIODrv_setDATAOE(uint8_t state){
  if (state){
    nDATA_OE_LATbits.nDATA_OE_LATPIN = 1;
  }
  else{
    nDATA_OE_LATbits.nDATA_OE_LATPIN = 0;
  }
}

// 165 shift register - SHIFT_LOAD, SHIFT_CLK, SHIFT_OUT (input)
void GPIODrv_setSHIFTLOAD(uint8_t state){
  if (state){
    SHIFT_LOAD_LATbits.SHIFT_LOAD_LATPIN = 1;
  }
  else{
    SHIFT_LOAD_LATbits.SHIFT_LOAD_LATPIN = 0;
  }
}

void GPIODrv_setSHIFTCLK(uint8_t state){
  if (state){
    SHIFT_CLK_LATbits.SHIFT_CLK_LATPIN = 1;
  }
  else{
    SHIFT_CLK_LATbits.SHIFT_CLK_LATPIN = 0;
  }
}

uint32_t GPIODrv_getSHIFTOUT(){
  return SHIFT_OUT_PORTbits.SHIFT_OUT_PORTPIN;
}