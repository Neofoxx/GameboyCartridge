#include <p32xxxx.h>
#include <inttypes.h>
#include <COMMS.h>
#include <usb.h>
#include <string.h>
#include <stdio.h>
#include <LED.h>
#include <GPIODrv.h>
#include <configBits.h>

// This file handles communication

// Circular buffers, all same size.
// uartTX is our sending to the PC
// uartRX is us receiving from the PC
// TODO - initialize all to 0, don't trust the compiler.
comStruct uartRXstruct;	// Target to PC
comStruct uartTXstruct;	// PC to target


uint32_t COMMS_USB_uartTX_transmitBuf(){
	// Called from USB routine, to send data we received from target

	comStruct* whichStruct = &uartTXstruct;	// Copy&paste error protection
	uint32_t sizeToSend = COMMS_helper_dataLen(whichStruct);
	uint8_t *buf;

	if (sizeToSend > EP_UARTINTERFACE_NUM_LEN){
		sizeToSend = EP_UARTINTERFACE_NUM_LEN;	// Limit to size of endpoint
	}

	if (sizeToSend == 0){
		if (whichStruct->sizeofLastSent == EP_UARTINTERFACE_NUM_LEN){
			// If we landed on a boundary last time, send a zero-length packet
			usb_send_in_buffer(EP_UARTINTERFACE_NUM, 0);
			whichStruct->sizeofLastSent = 0;
			return 0;	// Return - packet used up
		}
		else{
			// Do nothing. Nothing to send, no transaction to complete.
			return 1;	// Return - nothing to be done
		}
	}
	else{
		buf = usb_get_in_buffer(EP_UARTINTERFACE_NUM);			// Get buffer from endpoint
		COMMS_helper_getData(whichStruct, sizeToSend, buf);	// Get sizeToSend data and copy into buf
		usb_send_in_buffer(EP_UARTINTERFACE_NUM, sizeToSend);	// Send on endpoint EP_UART_NUM, of length sizeToSend
		whichStruct->sizeofLastSent = sizeToSend;		// Save data size, so we can finish transaction if needed
		return 0;		// Return - packet used up
	}
}

// UART TX - add data from USB to UART TX
uint32_t COMMS_uartRX_addToBuf(){
	// Called from USB, to gives us data from PC
	comStruct* whichStruct = &uartRXstruct;	// Copy&paste error protection
	const unsigned char *out_buf;
	volatile size_t out_buf_len;


	// Check for an empty transaction.
	out_buf_len = usb_get_out_buffer(EP_UARTINTERFACE_NUM, &out_buf);
	if ( (out_buf_len <= 0)){
		usb_arm_out_endpoint(EP_UARTINTERFACE_NUM);
		return 0;	// Return - packet used up
	}
	else{
		if (COMMS_helper_addToBuf(whichStruct, (uint8_t *)out_buf, out_buf_len)){
			return 1;	// Return - nothing to be done
		}
	}

	usb_arm_out_endpoint(EP_UARTINTERFACE_NUM);

	return 0;	// Return - packet used up

}

// HELPER FUNCTION
// helper function to add to buffer, so don't have to deal with hard coded things etc.
uint32_t COMMS_helper_addToBuf(comStruct* st, uint8_t* data, uint16_t length){
	if (COMMS_helper_spaceLeft(st) < length){
		return 1;	// Fail
	}

	uint32_t i = 0;
	for (i=0; i < length; i++){
		st->data[st->head] = data[i];
		st->head = (st->head + 1) & cyclicBufferSizeMask;
	}

	return 0; // 0 on success, else otherwise (no space available)
}

// Returns how much data is in the struct
inline uint32_t COMMS_helper_dataLen(comStruct* st){
	return (st->head - st->tail) & cyclicBufferSizeMask;
}

// Returns how much space is left in the struct
inline uint32_t COMMS_helper_spaceLeft(comStruct* st){
	return (st->tail - st->head - 1) & cyclicBufferSizeMask;
}

inline void COMMS_helper_getData(comStruct* st, uint32_t length, uint8_t *buf){
	if (COMMS_helper_dataLen(st) < length){
		// Don't do this, check beforehand
	}
	uint32_t i = 0;
	for (i=0; i< length; i++){
		buf[i] = st->data[st->tail];
		st->tail = (st->tail + 1) & cyclicBufferSizeMask;
	}
}

// ??
inline void COMMS_helper_peekData(const uint8_t *inData, uint32_t start, uint32_t length, uint8_t * buf){
	uint32_t counter = 0;
	for (counter = 0; counter < length; counter++){
		buf[counter] = inData[(start + counter) & cyclicBufferSizeMask];	// Also takes care of if start is already out of bounds.
	}
}

// Check if there is a newline available
int32_t COMMS_helper_checkNewline(comStruct* st){
	int32_t elements = -1;
	uint32_t maxIndex = COMMS_helper_dataLen(st);
	for (uint32_t i=0; i< maxIndex; i++){
		if (st->data[(st->tail + i) & cyclicBufferSizeMask] == '\n'){
			elements = i+1;
			break;
		}
	}

	return elements;
}

void COMMS_send(char* data, uint16_t len){
	COMMS_helper_addToBuf(&uartTXstruct, (uint8_t *)data, len);
}