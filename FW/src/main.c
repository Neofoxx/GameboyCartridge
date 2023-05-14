#include <p32xxxx.h>
#include <const.h>      // MIPS32
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <newlib.h>
#include <errno.h>

// Config bits
#include <configBits.h>

// Drivers for HW
#include <GPIODrv.h>
#include <LED.h>
// USB
#include <usb.h>
#include <usb_config.h>
#include <usb_ch9.h>
#include <usb_cdc.h>
// COMMS - usb and UART
#include <COMMS.h>
// Interrupts
#include <interrupt.h>

// Commands
#include <commandExecutor.h>

// Delay
#include <support.h>

char inBuf[2048];

// Not actually used
static struct cdc_line_coding line_coding =
{
	115200,
	CDC_CHAR_FORMAT_1_STOP_BIT,
	CDC_PARITY_NONE,
	8,
};

volatile uint32_t packetCounter = 0;	// Reset by SOF. May be useful


// TODO - run the timer, like in MX440 example (SysTick style)
// TODO2 - there should be an implementation, that uses the 24MHz/2 clock using CP0
void simpleDelay(unsigned int noOfLoops){
    unsigned int i = 0;
    while (i<noOfLoops){
        i++;
        asm("nop");
    }
}

void setup(){

	// Disable JTAG pins!

	GPIODrv_init();
	LED_init();
	
    // Check, there was a bug in one of the versions...
	//uint32_t * ANSELB_REAL = (uint32_t *)0xBF802BB0;
	//ANSELB_REAL[0] = 0;

	// Copied for USB, from hardware.c
	IPC7bits.USBIP = 4;

	// Enable interrupts - at the end, otherwise setting priorities is ineffective ? TODO
	//INTEnableSystemMultiVectoredInt();
	// PIC32MM needs some hand work
	SYSKEY = 0x00000000;      // force lock
	SYSKEY = 0xAA996655;      // unlock
	SYSKEY = 0x556699AA; 

	SPLLCON = (0b010 << 24)	// PLLODIV = PLL divide-by-4, so we get 24MHZ from 96MHz
		| (0b0000101 << 16)		// PLLMULT = x12, so 8MHz * 12 = 96MHz
		| (0b1 << 7);			// FRC is selected as input to PLL

	simpleDelay(1000);

	OSCCONbits.NOSC = 1;	// Switch to SPLL
	
	OSCCONSET = 1;		// Sets the OSWEN bit, that forces a change to news settings in NOSC
	
	SYSKEY = 0x00000000;    // force lock

	simpleDelay(100000);


	// Setup oscillator tuning, because we're too cool for crystals
	SYSKEY = 0x00000000;      // force lock
	SYSKEY = 0xAA996655;      // unlock
	SYSKEY = 0x556699AA; 
	
	OSCTUN = (1<<12) 		// SOSC host clock used to tune
		| (0b100000 << 0);	// Set to -1.5% off, just to see on scope.	
		
	simpleDelay(400000);	
		
	OSCTUNSET = (1<<15);	// Set ON bit, to start tuning.	
	
	SYSKEY = 0x00000000;      // force lock

	simpleDelay(100000);
	
	REFO1TRIM = 0;
	
	REFO1CON = (48 << 16)	// Divde input by 2*(val + TRIM/512). 2 == 4
		| (1 << 15)			// ON bit
		| (0 << 12)			// NOT Driven on REFO1 pin
		| (1 << 11)			// Continues in sleep mode
		| (0b0111);			// PLL VCO is input to REFCLK
	
	
	
	
	// Basically INTEnableSystemMultiVectoredInt
	// Enable multi-vectored mode
	//INTCONSET = _INTCON_MVEC_MASK;
	//INTCON = INTCON | _INTCON_MVEC_MASK;
	INTCONbits.MVEC=1; 

	// Interrupt exceptions use their own dedication exception vector offset (0x200), when CAUSE_IV is set to 1
	_CP0_BIS_CAUSE(_CP0_CAUSE_IV_MASK);

	// set the CP0 status IE bit high to turn on interrupts
	//INTEnableInterrupts();
	asm("ei");
	
}


int main(){

	setup();
	usb_init();
	
	IEC0bits.USBIE = 1;

	char outBuf[128];
	int len;

	int counter = 0;



	for(;;){
	
		len = sprintf(outBuf, "This is a test %d %d\n", counter++, COMMS_helper_dataLen(&uartRXstruct));

		//COMMS_helper_addToBuf(&uartTXstruct, (uint8_t *)outBuf, len);

		// At this point, check if there is a \n in the input buffer
		// If there isn't a \n for x chars, clear buffer or something
		// If there is, go parse the string.

		//delayMs(500);

		int32_t elements = COMMS_helper_checkNewline(&uartRXstruct);
		if (elements > 0){
			COMMS_helper_getData(&uartRXstruct, elements, (uint8_t *)inBuf);
			inBuf[elements] = 0;		// Null terminate
			inBuf[elements - 1] = 0;	// Delete newline

			commandExecutor(inBuf);

		}

		// Unused
		#ifndef USB_USE_INTERRUPTS
		usb_service();
		#endif

	}

    return(0);
    
}




/////////////////////////////////////////////////////////////7

#ifdef USB_USE_INTERRUPTS
#if defined (__PIC32MX__)

INTERRUPT(USB1Interrupt){

#elif defined (__PIC32MM__)

INTERRUPT(USBInterrupt){

#endif
  usb_service();  // Deal with USB stuff

  // All of the cases depend on this anyway.
  if (!usb_is_configured()){
    return;
  }

  // Priorities!

  // 3. We have one IN (us->PC) endpoint 
  // 2. We have one OUT (PC->us) endpoint


  ////////////////////////////////////////////////////////////
  // 2. Get data from EP2 OUT (PC to us)
  ////////////////////////////////////////////////////////////
  if (!usb_out_endpoint_halted(EP_UARTINTERFACE_NUM) && usb_out_endpoint_has_data(EP_UARTINTERFACE_NUM) && !usb_in_endpoint_busy(EP_UARTINTERFACE_NUM)) {

    if (!COMMS_uartRX_addToBuf()){
      // Packet is consumed regardless
    }

  }

  ////////////////////////////////////////////////////////////
  // 3. Push data to EP2 IN (us to PC)
  ////////////////////////////////////////////////////////////

  // Post data to EP2 IN (us to PC)
  if (!usb_in_endpoint_halted(EP_UARTINTERFACE_NUM) && !usb_in_endpoint_busy(EP_UARTINTERFACE_NUM)){
    if (!COMMS_USB_uartTX_transmitBuf()){
      // Packet is consumed regardless
    }
  }

}
#endif


/* Callbacks. These function names are set in usb_config.h. */
void app_set_configuration_callback(uint8_t configuration)
{

}

uint16_t app_get_device_status_callback()
{
	return 0x0000;
}

void app_endpoint_halt_callback(uint8_t endpoint, bool halted){
	// This is a notification only.  There is no way to reject this request.
}

int8_t app_set_interface_callback(uint8_t interface, uint8_t alt_setting){
	// The callback should return 0 if the new alternate setting can be set or -1 if
	// it cannot. This callback is completely unnecessary if you only have one
	// alternate setting (alternate setting zero) for each interface.
	return 0;
}

int8_t app_get_interface_callback(uint8_t interface){
	// The application should return the interface's current alternate setting
	return 0;
}

void app_out_transaction_callback(uint8_t endpoint){
	// If we wanted, we can use this function to trigger when the
	// OUT transaction on the endpoint is finished (PC->us), and read
	// data out fast that way. Unnecessary for now.
}

void app_in_transaction_complete_callback(uint8_t endpoint){
	// If we wanted, we can use this function to trigger when the
	// IN transaction on the endpoint is finished (us->PC), and rearm/send
	// data fast that way. Unnecessary for now.
}

int8_t app_unknown_setup_request_callback(const struct setup_packet *setup){
	/* To use the CDC device class, have a handler for unknown setup
	 * requests and call process_cdc_setup_request() (as shown here),
	 * which will check if the setup request is CDC-related, and will
	 * call the CDC application callbacks defined in usb_cdc.h. For
	 * composite devices containing other device classes, make sure
	 * MULTI_CLASS_DEVICE is defined in usb_config.h and call all
	 * appropriate device class setup request functions here.
	 */
	// Since we only use (dual) CDC, cdc_setup_request is enough, it gets called twise.
	return process_cdc_setup_request(setup);
}

int16_t app_unknown_get_descriptor_callback(const struct setup_packet *pkt, const void **descriptor){
	// Called when a GET_DESCRIPTOR request is received from
	// the host for a descriptor which is unrecognized by the USB stack.
	return -1;
}

void app_start_of_frame_callback(void){
	// Interrupt at SOF
}

void app_usb_reset_callback(void){
	// Possibly reset USB buffer, or setup DMAs or something.
	// Unnecessary for now.
}

/* CDC Callbacks. See usb_cdc.h for documentation. */

int8_t app_send_encapsulated_command(uint8_t interface, uint16_t length){
	// Unsure what to support here.
	return -1;
}

int16_t app_get_encapsulated_response(uint8_t interface,
                                      uint16_t length, const void **report,
                                      usb_ep0_data_stage_callback *callback,
                                      void **context){
	// Unsure what to support here.
	return -1;
}

int8_t app_set_comm_feature_callback(uint8_t interface, bool idle_setting, bool data_multiplexed_state){
  return -1;
}

int8_t app_clear_comm_feature_callback(uint8_t interface, bool idle_setting, bool data_multiplexed_state){
  return -1; 
}

int8_t app_get_comm_feature_callback(uint8_t interface, bool *idle_setting, bool *data_multiplexed_state){
  return -1;
}

int8_t app_set_line_coding_callback(uint8_t interface, const struct cdc_line_coding *coding){

  // Check if values are in ranges we support
  // Technically we should just ignore this, but leave in as is anyway
  if (coding->dwDTERate <= 1000000
    && (coding->bCharFormat == CDC_CHAR_FORMAT_1_STOP_BIT || coding->bCharFormat == CDC_CHAR_FORMAT_2_STOP_BITS)
    && coding->bParityType == CDC_PARITY_NONE
    && coding->bDataBits == 8){

    line_coding = *coding;
  }

  return 0;	// No way to deny. Must return 0, otherwise it will stall.
}

int8_t app_get_line_coding_callback(uint8_t interface, struct cdc_line_coding *coding){
  // Return what sort of line_coding we are using.
  *coding = line_coding;
  return 0;
}

int8_t app_set_control_line_state_callback(uint8_t interface, bool dtr, bool rts){
  // Set DTR and RTS lines, according to demands.
  // No interface currently supports this

  return 0;
}

int8_t app_send_break_callback(uint8_t interface, uint16_t duration){
	return 0;
}



