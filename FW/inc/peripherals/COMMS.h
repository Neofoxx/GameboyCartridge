#ifndef COMMS_H_96582fe1b9194228b1c0475bf8856698
#define COMMS_H_96582fe1b9194228b1c0475bf8856698


#define cyclicBufferSize		2048
#define cyclicBufferSizeMask	(cyclicBufferSize-1)
// Check to enforce buffer being of size 2^n
_Static_assert( ((cyclicBufferSize & cyclicBufferSizeMask) == 0), "Buffer size not equal to 2^n"); // TODO get rid of Eclipse warning.


// To replace struct above!
typedef struct comStruct{
	uint8_t data[cyclicBufferSize];
	uint16_t head;
	uint16_t tail;
	uint32_t timeStamp;
	uint32_t sizeofLastSent;
} comStruct;

extern comStruct uartRXstruct;	// PC to target
extern comStruct uartTXstruct;	// Us to PC (Return values)

uint32_t COMMS_helper_addToBuf(comStruct* st, uint8_t* data, uint16_t length);
uint32_t COMMS_helper_dataLen(comStruct* st);

uint32_t COMMS_helper_spaceLeft(comStruct* st);
void COMMS_helper_getData(comStruct* st, uint32_t length, uint8_t *buf);
void COMMS_helper_peekData(const uint8_t *inData, uint32_t start, uint32_t length, uint8_t * buf);
int32_t COMMS_helper_checkNewline(comStruct* st);

uint32_t COMMS_uartRX_addToBuf();
uint32_t COMMS_USB_uartTX_transmitBuf();

// Helper function, looks nicer
void COMMS_send(char* data, uint16_t len);

#endif
