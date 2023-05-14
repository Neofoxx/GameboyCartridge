#ifndef GPIODRV_H_573f2ca8b43e44209acc90f0f9b7de5d
#define GPIODRV_H_573f2ca8b43e44209acc90f0f9b7de5d

#include <p32xxxx.h>
#include <inttypes.h>

#define GPIO_LOW		0
#define GPIO_HIGH		1

// LED_1
#define LED_1_TRISbits	    TRISBbits
#define LED_1_TRISPIN	    TRISB0
#define LED_1_LATbits	    LATBbits
#define LED_1_LATPIN	    LATB0
#define LED_1_LATINV	    LATBINV
#define LED_1_MASK	    (1<<0)
#define LED_1_ANSEL_REG     ANSELBbits
#define LED_1_ANSEL_BIT     ANSB0

// LED_2
#define LED_2_TRISbits	    TRISBbits
#define LED_2_TRISPIN	    TRISB1
#define LED_2_LATbits	    LATBbits
#define LED_2_LATPIN	    LATB1
#define LED_2_LATINV        LATBINV
#define LED_2_MASK          (1<<1)
#define LED_2_ANSEL_REG     ANSELBbits
#define LED_2_ANSEL_BIT     ANSB1

// LED_3 / WR
#define WR_TRISbits	    TRISBbits
#define WR_TRISPIN	    TRISB2
#define WR_LATbits	    LATBbits
#define WR_LATPIN	    LATB2
#define WR_LATINV	    LATBINV
#define WR_MASK		    (1<<2)
#define WR_ANSEL_REG        ANSELBbits
#define WR_ANSEL_BIT        ANSB2

// LED_4 / RD
#define RD_TRISbits	    TRISBbits
#define RD_TRISPIN	    TRISB3
#define RD_LATbits	    LATBbits
#define RD_LATPIN	    LATB3
#define RD_LATINV           LATBINV
#define RD_MASK             (1<<3)
#define RD_ANSEL_REG        ANSELBbits
#define RD_ANSEL_BIT        ANSB3

// GB_RESET pin
#define GB_RESET_TRISbits   TRISAbits
#define GB_RESET_TRISPIN    TRISA3
#define GB_RESET_LATbits    LATAbits
#define GB_RESET_LATPIN	    LATA3
#define GB_RESET_LATINV	    LATAINV
#define GB_RESET_MASK	    (1<<3)

// EXT 1-3 pins, used for SPI slave (someday...)
#define EXT_1_TRISbits	        TRISAbits
#define EXT_1_TRISPIN		TRISA2
#define EXT_1_LATbits	        LATAbits
#define EXT_1_LATPIN		LATA2
#define EXT_1_LATINV		LATAINV
#define EXT_1_MASK		(1<<2)

#define EXT_2_TRISbits	        TRISBbits
#define EXT_2_TRISPIN		TRISB5
#define EXT_2_LATbits	        LATBbits
#define EXT_2_LATPIN		LATB5
#define EXT_2_LATINV		LATAINV
#define EXT_2_MASK		(1<<5)

#define EXT_3_TRISbits	        TRISBbits
#define EXT_3_TRISPIN		TRISB7
#define EXT_3_LATbits	        LATBbits
#define EXT_3_LATPIN		LATB7
#define EXT_3_LATINV		LATBINV
#define EXT_3_MASK		(1<<7)

// 74HC165 Paralell-to-Serial shifter (SPI?)
#define SHIFT_LOAD_TRISbits     TRISBbits
#define SHIFT_LOAD_TRISPIN      TRISB14
#define SHIFT_LOAD_LATbits      LATBbits
#define SHIFT_LOAD_LATPIN       LATB14
#define SHIFT_LOAD_LATINV       LATBINV
#define SHIFT_LOAD_MASK	        (1<<14)
#define SHIFT_LOAD_ANSEL_REG    ANSELBbits
#define SHIFT_LOAD_ANSEL_BIT    ANSB14

#define SHIFT_CLK_TRISbits      TRISBbits
#define SHIFT_CLK_TRISPIN        TRISB13
#define SHIFT_CLK_LATbits       LATBbits
#define SHIFT_CLK_LATPIN        LATB13
#define SHIFT_CLK_LATINV        LATBINV
#define SHIFT_CLK_MASK	        (1<<13)
#define SHIFT_CLK_ANSEL_REG     ANSELBbits
#define SHIFT_CLK_ANSEL_BIT     ANSB13

#define SHIFT_OUT_TRISbits      TRISBbits
#define SHIFT_OUT_TRISPIN       TRISB15
#define SHIFT_OUT_LATbits       LATBbits
#define SHIFT_OUT_LATPIN        LATB15
#define SHIFT_OUT_LATINV        LATBINV
#define SHIFT_OUT_MASK	        (1<<15)
#define SHIFT_OUT_PORTbits      PORTBbits
#define SHIFT_OUT_PORTPIN       RB15
#define SHIFT_OUT_ANSEL_REG     ANSELBbits
#define SHIFT_OUT_ANSEL_BIT     ANSB15

// 74HC595 Serial-in Paralell-out shifters, 3 in chain. (SPI?)
// nADDR_OE - output enable for the two register on the address pins
#define nADDR_OE_TRISbits       TRISBbits
#define nADDR_OE_TRISPIN         TRISB4
#define nADDR_OE_LATbits        LATBbits
#define nADDR_OE_LATPIN         LATB4
#define nADDR_OE_LATINV         LATBINV
#define nADDR_OE_MASK	        (1<<4)
// nDATA_OE - output enable for the one register on the data pins
#define nDATA_OE_TRISbits       TRISAbits
#define nDATA_OE_TRISPIN         TRISA4
#define nDATA_OE_LATbits        LATAbits
#define nDATA_OE_LATPIN         LATA4
#define nDATA_OE_LATINV         LATAINV
#define nDATA_OE_MASK	        (1<<4)

#define REG_CLK_TRISbits        TRISCbits
#define REG_CLK_TRISPIN         TRISC9
#define REG_CLK_LATbits         LATCbits
#define REG_CLK_LATPIN          LATC9
#define REG_CLK_LATINV          LATCINV
#define REG_CLK_MASK	        (1<<9)

#define SHIFT_IN_TRISbits       TRISBbits
#define SHIFT_IN_TRISPIN        TRISB9
#define SHIFT_IN_LATbits        LATBbits
#define SHIFT_IN_LATPIN         LATB9
#define SHIFT_IN_LATINV         LATBINV
#define SHIFT_IN_MASK	        (1<<9)

#define DATA_CLK_TRISbits       TRISBbits
#define DATA_CLK_TRISPIN        TRISB8
#define DATA_CLK_LATbits        LATBbits
#define DATA_CLK_LATPIN         LATB8
#define DATA_CLK_LATINV         LATBINV
#define DATA_CLK_MASK	        (1<<8)

// Function definitons
void GPIODrv_init();

void GPIODrv_setRD(uint8_t state);
void GPIODrv_setWR(uint8_t state);
void GPIODrv_setRST(uint8_t state);

// 595 shift register - SHIFT_IN, DATA_CLK, REG_CLK, ADDR_OE, DATA_OE
void GPIODrv_setSHIFTIN(uint8_t state);
void GPIODrv_setDATACLK(uint8_t state);
void GPIODrv_setREGCLK(uint8_t state);
void GPIODrv_setADDROE(uint8_t state);
void GPIODrv_setDATAOE(uint8_t state);

// 165 shift register - SHIFT_LOAD, SHIFT_CLK, SHIFT_OUT (input)
void GPIODrv_setSHIFTLOAD(uint8_t state);
void GPIODrv_setSHIFTCLK(uint8_t state);
uint32_t GPIODrv_getSHIFTOUT();


#endif
