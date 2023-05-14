#include <commandExecutor.h>
#include <string.h>
#include <stdio.h>
#include <COMMS.h>
#include <ctype.h>
#include <stdlib.h>
#include <LED.h>
#include <GPIODrv.h>
#include <support.h>

char outBuf [256];

void stringUp(char* inString){
    while(inString[0] != '\0'){
        inString[0] = toupper(inString[0]);
        inString++;
    }
}

void sendOK(){
    int len = sprintf(outBuf, "OK\n");
    COMMS_send(outBuf, len);
}

void sendNACK(){
    int len = sprintf(outBuf, "NACK\n");
    COMMS_send(outBuf, len);
}

// inString must be null terminated
void commandExecutor(char* inString){
    const char delimiter[] = " ";
    char* token;
    int len = 0;

    stringUp(inString);
    memset(outBuf, 0, sizeof(outBuf));

    token = strtok(inString, delimiter);

    if (0 == strcmp(token, "HELP")){
        len = len + sprintf(outBuf + len, "Cartridge!\n");
        len = len + sprintf(outBuf + len, "---------------------------\n");
        len = len + sprintf(outBuf + len, "32kB \"ROM\", 8kB \"RAM\"\n");
        COMMS_send(outBuf, len);
    }

    // Allows overriding any pin. Also allows writing programming algorithms on the PC side, albeit slower
    else if (0 == strcmp(token, "SET")){
        char* pin = strtok(NULL, delimiter);    // PIN
        char* valStr = strtok(NULL, delimiter);    // Value

        int32_t val = atoi(valStr); // Just 0 or 1, so we could do it more nicely

        if (0 == strcmp(pin, "LED_1")){
            LED_1_setState(val);
            sendOK();
        }
        else if (0 == strcmp(pin, "LED_2")){
            LED_2_setState(val);
            sendOK();
        }

        // Cartridge connector pins
        else if (0 == strcmp(pin, "RD")){
            GPIODrv_setRD(val);
            sendOK();
        }
        else if (0 == strcmp(pin, "WR")){
            GPIODrv_setWR(val);
            sendOK();
        }
        else if (0 == strcmp(pin, "RST")){
            GPIODrv_setRST(val);
            sendOK();
        }

        // 595 shift register
        else if (0 == strcmp(pin, "SHIFTIN")){
            GPIODrv_setSHIFTIN(val);
            sendOK();
        }
        else if (0 == strcmp(pin, "DATACLK")){
            GPIODrv_setDATACLK(val);
            sendOK();
        }
        else if (0 == strcmp(pin, "REGCLK")){
            GPIODrv_setREGCLK(val);
            sendOK();
        }
        else if (0 == strcmp(pin, "ADDROE")){
            GPIODrv_setADDROE(val);
            sendOK();
        }
        else if (0 == strcmp(pin, "DATAOE")){
            GPIODrv_setDATAOE(val);
            sendOK();
        }

        // 165 register
        else if (0 == strcmp(pin, "SHIFTLOAD")){
            GPIODrv_setSHIFTLOAD(val);
            sendOK();
        }
        else if (0 == strcmp(pin, "SHIFTCLK")){
            GPIODrv_setSHIFTCLK(val);
            sendOK();
        }

        else{
            sendNACK();
        }

        
    }

    else if (0 == strcmp(token, "GET")){
        char* pin = strtok(NULL, delimiter);    // PIN

        if (0 == strcmp(pin, "DATAOE")){
            len = len + sprintf(outBuf + len, "VAL: %ld\n", GPIODrv_getSHIFTOUT());
            COMMS_send(outBuf, len);
        }

        else{
            sendNACK();
        }
    }
    
    // Write a B to the address
    else if (0 == strcmp(token, "WRITEB")){
        char* addr = strtok(NULL, delimiter);   // 16-bit address
        char* data = strtok(NULL, delimiter);   // 8-bit data

        // worry about the ul later
        uint16_t address = strtoul(addr, NULL, 16);
        uint8_t byteToWrite;

        // int len = 0;

        // len = len + sprintf(outBuf + len, "%02X ", address);

        do{
            byteToWrite = strtoul(data, NULL, 16);

            // len = len + sprintf(outBuf + len, "%02X ", byteToWrite);

            cartridgeWriteByte(address, byteToWrite);
            data = strtok(NULL, delimiter);   // 8-bit data

            address++;  // !

        }while(data != NULL);

        // len = len + sprintf(outBuf + len, "\n");
        // COMMS_send(outBuf, len);
        sendOK();
    }

    // Read a B from the address
    else if (0 == strcmp(token, "READB")){
        char* addr = strtok(NULL, delimiter);   // 16-bit address
        char* data = strtok(NULL, delimiter);   // possible length,

        uint16_t address = strtoul(addr, NULL, 16);
        uint16_t lengthToRead = 1;

        if (data != NULL){
            lengthToRead = strtoul(data, NULL, 16);
        }

        int len = 0;
        len = len + sprintf(outBuf + len, "OK");

        int i = 0;
        for (i = 0; i<lengthToRead; i++){
            len = len + sprintf(outBuf + len, " %02X", cartridgeReadByte(address + i));
        }

        len = len + sprintf(outBuf + len, "\n");

        COMMS_send(outBuf, len);             

    }


}

uint8_t cartridgeReadByte(uint16_t address){
    // First part is setting up the 595, then reading the 165
    GPIODrv_setADDROE(1);   // Active LOW
    GPIODrv_setDATAOE(1);   // Active LOW
    GPIODrv_setREGCLK(0);   // Active HIGH. On LOW->HIGH data from internal register is output
    GPIODrv_setDATACLK(0);  // Active HIGH. On LOW->HIGH data from INPUT is transferred into Q0, Q0->Q1, etc.
    GPIODrv_setSHIFTIN(0);  // Our data input

    GPIODrv_setRD(0);   // Active HIGH
    GPIODrv_setWR(0);   // Active HIGH

    // 165 setup
    GPIODrv_setSHIFTLOAD(1);   // Active LOW
    GPIODrv_setSHIFTCLK(0);    // Active HIGH
        

    // The 505 has a min FREQ of 9MHz at 2V, 30MHz at 4.5V
    delayUs(1);

    // Data is shifted MSB first into the registers.
    // But.
    // Shift registers are [A0-A7] -> [A8-A15] -> [D0-D7]
    // So first clock the data, then the address.
    uint32_t tempData = (uint32_t)address;
    uint32_t i = 0;
    for (i=0; i< 24; i++){
        uint32_t bitToShift = (tempData >> (23 - i)) & 0x01;
        GPIODrv_setSHIFTIN(bitToShift);
        delayUs(1);
        GPIODrv_setDATACLK(1);
        delayUs(1);
        GPIODrv_setDATACLK(0);
        delayUs(1);
    }
    GPIODrv_setREGCLK(1);
    delayUs(1);
    GPIODrv_setREGCLK(0);
    delayUs(1);
    
    // Just ADDR this time
    GPIODrv_setADDROE(0);   // Active LOW
    delayUs(1);

    GPIODrv_setRD(1);   // Active HIGH
    delayUs(1);        // TODO check

    // Read the 165 here
    GPIODrv_setSHIFTLOAD(0);   // Active LOW
    delayUs(1);                // TODO check
    GPIODrv_setSHIFTLOAD(1);   // Active LOW
    delayUs(1);                // TODO check

    uint8_t retVal = 0;
    for (i = 0; i<8; i++){
        retVal = retVal | (GPIODrv_getSHIFTOUT() << (7-i));
        GPIODrv_setSHIFTCLK(1);    // Active HIGH
        delayUs(1);                // TODO check
        GPIODrv_setSHIFTCLK(0);    // Active HIGH
        delayUs(1);                // TODO check
    }

    GPIODrv_setRD(0);   // Active HIGH
    delayUs(1);        // TODO check
    GPIODrv_setADDROE(1);   // Active LOW
    delayUs(1);

    return retVal;
}


void cartridgeWriteByte(uint16_t address, uint8_t data){
    // This expects that the register is in a known state previously
    // Eh, setup anyway
    GPIODrv_setADDROE(1);   // Active LOW
    GPIODrv_setDATAOE(1);   // Active LOW
    GPIODrv_setREGCLK(0);   // Active HIGH. On LOW->HIGH data from internal register is output
    GPIODrv_setDATACLK(0);  // Active HIGH. On LOW->HIGH data from INPUT is transferred into Q0, Q0->Q1, etc.
    GPIODrv_setSHIFTIN(0);  // Our data input

    GPIODrv_setRD(0);   // Active HIGH
    GPIODrv_setWR(0);   // Active HIGH
    

    // The 505 has a min FREQ of 9MHz at 2V, 30MHz at 4.5V
    //delayUs(1);

    // Data is shifted MSB first into the registers.
    // But.
    // Shift registers are [A0-A7] -> [A8-A15] -> [D0-D7]
    // So first clock the data, then the address.
    uint32_t tempData = (uint32_t)address | (((uint32_t)data) << 16);
    uint32_t i = 0;
    for (i=0; i< 24; i++){
        uint32_t bitToShift = (tempData >> (23 - i)) & 0x01;
        GPIODrv_setSHIFTIN(bitToShift);
        //delayUs(1);
        GPIODrv_setDATACLK(1);
        //delayUs(1);
        GPIODrv_setDATACLK(0);
        //delayUs(1);
    }
    GPIODrv_setREGCLK(1);
    //delayUs(1);
    GPIODrv_setREGCLK(0);
    //delayUs(1);
    
    GPIODrv_setADDROE(0);   // Active LOW
    GPIODrv_setDATAOE(0);   // Active LOW
    //delayUs(1);

    GPIODrv_setWR(1);   // Active HIGH
    //delayUs(1);        // TODO check
    GPIODrv_setWR(0);   // Active HIGH
    //delayUs(1);        // TODO check

    GPIODrv_setADDROE(1);   // Active LOW
    GPIODrv_setDATAOE(1);   // Active LOW
    //delayUs(1);

}