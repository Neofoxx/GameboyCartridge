#include <p32xxxx.h>
#include <inttypes.h>
#include <configBits.h>

#if defined (__32MX270F256D__)
	const uint32_t __attribute__((section (".SECTION_DEVCFG3"))) temp3 =
		0xFFF0000
		| (0b0 << _DEVCFG3_FVBUSONIO_POSITION)	// USBVBUSON controlled by PORT function
		| (0b0 << _DEVCFG3_FUSBIDIO_POSITION)	// USBID controlled by PORT function
		| (0b0 << _DEVCFG3_IOL1WAY_POSITION)		// Allow multiple reconfigurations of Peripheral Pins
		| (0b0 << _DEVCFG3_PMDL1WAY_POSITION)	// Allow multiple reconfigurations of Peripheral Module Disable
		| (0xF0C5 << _DEVCFG3_USERID_POSITION);	// UserID is F0C5

	const uint32_t __attribute__((section (".SECTION_DEVCFG2"))) temp2 =
		0xFFF87888
		| (0b001 << _DEVCFG2_FPLLODIV_POSITION)	// PLL output divided by 2 (96MHz/2 = 48Mhz)
		| (0b0 << _DEVCFG2_UPLLEN_POSITION)		// USB PLL Enabled
		| (0b001 << _DEVCFG2_UPLLIDIV_POSITION)	// USB PLL input divided by 2 (8MHz/2 = 4MHz)
		| (0b111 << _DEVCFG2_FPLLMUL_POSITION)	// PLL Multiplier is 24 (4MHz*24 = 96MHz)
		| (0b001 << _DEVCFG2_FPLLIDIV_POSITION);	// PLL input divided by 2 (8MHz/2 = 4MHz)

	const uint32_t __attribute__((section (".SECTION_DEVCFG1"))) temp1 =
		0xFC200858
		| (0b00 << _DEVCFG1_FWDTWINSZ_POSITION)	// Watchdog timer window size is 75%
		| (0b0 << _DEVCFG1_FWDTEN_POSITION)		// Watchdog timer disabled, can be enabled in software
		| (0b00000 << _DEVCFG1_WDTPS_POSITION)	// Watchdog timer postscale is 1
		| (0b01 << _DEVCFG1_FCKSM_POSITION)		// Clock switching enabled, Fail-Safe Clock Monitoring DISABLED
		| (0b00 << _DEVCFG1_FPBDIV_POSITION)	// PBCLK is SYSCLK / 1
		| (0b1 << _DEVCFG1_OSCIOFNC_POSITION)	// CLOCK output disabled
		| (0b01 << _DEVCFG1_POSCMOD_POSITION)	// XT oscillator mode
		| (0b0 << _DEVCFG1_IESO_POSITION)		// Internal-External switchover disabled (Two-speed start-up disabled)
		| (0b1 << _DEVCFG1_FSOSCEN_POSITION)		// Enable secondary oscillator (WARNING; CHECK IF PORTING)
		| (0b011 << _DEVCFG1_FNOSC_POSITION);		// POSC (XT) + PLL selected

	const uint32_t __attribute__((section (".SECTION_DEVCFG0"))) temp0 =
		0x6EF803E0								// Don't forget about that one 0
		| (0b1 << _DEVCFG0_CP_POSITION)			// Code Protection disabled
		| (0b1 << _DEVCFG0_BWP_POSITION)			// Boot Flash is Writeable during code execution
		| (0b1111111111 << _DEVCFG0_PWP_POSITION)	// Memory is NOT write-protected
		| (0b00 << _DEVCFG0_ICESEL_POSITION)		// PGEC4/PGED4 is used
		| (0b1 << _DEVCFG0_JTAGEN_POSITION)		// JTAG is enabled
	#ifdef DEBUG_BUILD							// Defined with Makefile
		| (0b11<<_DEVCFG0_DEBUG_POSITION);		// Debugger is DISABLED. Apparently the MX1/MX2 family need this disabled, for JTAG to work
												// Note, application will run automatically, might want to add a delay at the beginning.
	#else
		| (0b11<<_DEVCFG0_DEBUG_POSITION);		// Debugger is DISABLED (DEBUG bit) - DEBUG NEEDS TO BE DISABLED, IF NO DEBUGGER PRESENT! Otherwise code doesn't run.
	#endif

#elif defined (__32MX440F256H__)

	const uint32_t __attribute__((section (".SECTION_DEVCFG3"))) temp3 = 0xFFFFBEEF;   // DEVCFG3
	const uint32_t __attribute__((section (".SECTION_DEVCFG2"))) temp2 =
		0xFFF87888
		| (0b000<<_DEVCFG2_FPLLODIV_POSITION)
		| (0b0<<_DEVCFG2_UPLLEN_POSITION)
		| (0b001<<_DEVCFG2_UPLLIDIV_POSITION)
		| (0b101<<_DEVCFG2_FPLLMUL_POSITION)
		| (0b001<<_DEVCFG2_FPLLIDIV_POSITION);   // DEVCFG2
	const uint32_t __attribute__((section (".SECTION_DEVCFG1"))) temp1 =
		0xFF600858
		| (0b10100<<_DEVCFG1_WDTPS_POSITION)
		| (0b01<<_DEVCFG1_FPBDIV_POSITION)
		| (0b0<<_DEVCFG1_OSCIOFNC_POSITION)
		| (0b10<<_DEVCFG1_POSCMOD_POSITION)
		| (0b0<<_DEVCFG1_IESO_POSITION)	//?
		| (0b0<<_DEVCFG1_FSOSCEN_POSITION)
		| (0b011<<_DEVCFG1_FNOSC_POSITION);   // DEVCFG1
	const uint32_t __attribute__((section (".SECTION_DEVCFG0"))) temp0 =
		0x6EF00FF4 // There's that _one_ bit that has to be 0
		| (0b1<<_DEVCFG0_CP_POSITION)
		| (0b1<<_DEVCFG0_BWP_POSITION)
		| (0b11111111<<_DEVCFG0_PWP_POSITION)
		| (0b1<<_DEVCFG0_ICESEL_POSITION)	// PGEC2/PGED2
	#ifdef DEBUG_BUILD	// Defined with Makefile
		| (0b10<<_DEVCFG0_DEBUG_POSITION);   // DEVCFG0
	#else
		| (0b11<<_DEVCFG0_DEBUG_POSITION);   // DEVCFG0	- DEBUG NEEDS TO BE DISABLED, IF NO DEBUGGER PRESENT! Otherwise code doesn't run.
	#endif
	
#elif defined (__32MM0256GPM036__)
	const uint32_t __attribute__((section (".SECTION_FDEVOPT"))) temp_fdevopt = 
		0x00003FE7
		| ((0xF0C5) << _FDEVOPT_USERID_POSITION)	// UserID is F0C5. NOTE! for some reason, it's declared as _17_ bits wide in the .h file. 		
													// Hence this hack, to have it aligned, since the definition shifts by 15, instead of 16...
		| (0b0 << _FDEVOPT_FVBUSIO_POSITION)	// VBUSON pin is controlled by port function, not USB
		| (0b0 << _FDEVOPT_FUSBIDIO_POSITION)	// USBID pin is controlled by port function, not USB	
		| (0b1 << _FDEVOPT_ALTI2C_POSITION)	// SDA1 and SCL1 are on pins RB8 and RB9											
		| (0b1 << _FDEVOPT_SOSCHP_POSITION);	// SOSC operated in Normal Power mode

	const uint32_t __attribute__((section (".SECTION_FICD"))) temp_ficd =
		0xFFFFFFE3
		| (0b10 << _FICD_ICS_POSITION)			// Communication is on PGEC2/PGED2
		| (0b1 << _FICD_JTAGEN_POSITION);		// JTAG is enabled

	const uint32_t __attribute__((section (".SECTION_FPOR"))) temp_fpor =
		0xFFFFFFF0
		| (0b0 << _FPOR_LPBOREN_POSITION)		// Low-Power BOR is disabled
		| (0b0 << _FPOR_RETVR_POSITION)			// Retention regulator is enabled, controlled by RETEN in sleep
		| (0b00 << _FPOR_BOREN_POSITION);		// Brown-out reset disabled in HW, SBOREN bit disabled

	const uint32_t __attribute__((section (".SECTION_FWDT"))) temp_fwdt =
		0xFFFF0000
		| (0b0 << _FWDT_FWDTEN_POSITION)		// Watchdog Timer disabled
		| (0b00 << _FWDT_RCLKSEL_POSITION)		// Clock source is system clock
		| (0b10000 << _FWDT_RWDTPS_POSITION)	// Run mode Watchdog Postscale is 1:65536
		| (0b1 << _FWDT_WINDIS_POSITION)		// Windowed mode is disabled
		| (0b00 << _FWDT_FWDTWINSZ_POSITION)	// Watchdog Timer window size is 75%
		| (0b10000 << _FWDT_SWDTPS_POSITION);	// Sleep mode Watchdog Postscale is 1:65536 

	const uint32_t __attribute__((section (".SECTION_FOSCSEL"))) temp_foscsel =
		0xFFFF2828
		| (0b01 << _FOSCSEL_FCKSM_POSITION)		// Clock switching ENABLED, Fail-Safe clock monitor disabled
		| (0b1 << _FOSCSEL_SOSCSEL_POSITION)	// Crystal is used for SOSC, RA4/RB4 controlled by SOSC
		| (0b1 << _FOSCSEL_OSCIOFNC_POSITION)	// System clock not on CLKO pin, operates as normal I/O
		| (0b11 << _FOSCSEL_POSCMOD_POSITION)	// Primary oscillator is disabled
		| (0b0 << _FOSCSEL_IESO_POSITION)		// Two-speed Start-up is disabled
		| (0b0 << _FOSCSEL_SOSCEN_POSITION)		// Secondary oscillator disabled
		| (0b1 << _FOSCSEL_PLLSRC_POSITION)		// FRC is input to PLL on device Reset
		| (0b000 << _FOSCSEL_FNOSC_POSITION);	// Oscillator is FRC with "Divide-by-N". We will switch to PLL later.

	const uint32_t __attribute__((section (".SECTION_FSEC"))) temp_fsec =
		0x7FFFFFFF
		| (0b1 << _FSEC_CP_POSITION);			// Code protection disabled
	// End PRIMARY configuration

	// Configuration bits for the PIC32MM chip. ALTERNATE configuration
	const uint32_t __attribute__((section (".SECTION_AFDEVOPT"))) temp_afdevopt = 
		0x0000FFF7
		| ((0xF0C5) << _FDEVOPT_USERID_POSITION)	// UserID is F0C5. NOTE! for some reason, it's declared as _17_ bits wide in the .h file. 
													// Hence this hack, to have it aligned, since the definition shifts by 15, instead of 16...
		| (0b1 << _FDEVOPT_SOSCHP_POSITION);	// SOSC operated in Normal Power mode

	const uint32_t __attribute__((section (".SECTION_AFICD"))) temp_aficd =
		0xFFFFFFE3
		| (0b01 << _FICD_ICS_POSITION)			// Communication is on PGEC3/PGED3
		| (0b1 << _FICD_JTAGEN_POSITION);		// JTAG is enabled

	const uint32_t __attribute__((section (".SECTION_AFPOR"))) temp_afpor =
		0xFFFFFFF0
		| (0b0 << _FPOR_LPBOREN_POSITION)		// Low-Power BOR is disabled
		| (0b0 << _FPOR_RETVR_POSITION)			// Retention regulator is enabled, controlled by RETEN in sleep
		| (0b00 << _FPOR_BOREN_POSITION);		// Brown-out reset disabled in HW, SBOREN bit disabled

	const uint32_t __attribute__((section (".SECTION_AFWDT"))) temp_afwdt =
		0xFFFF0000
		| (0b0 << _FWDT_FWDTEN_POSITION)		// Watchdog Timer disabled
		| (0b00 << _FWDT_RCLKSEL_POSITION)		// Clock source is system clock
		| (0b10000 << _FWDT_RWDTPS_POSITION)	// Run mode Watchdog Postscale is 1:65536
		| (0b1 << _FWDT_WINDIS_POSITION)		// Windowed mode is disabled
		| (0b00 << _FWDT_FWDTWINSZ_POSITION)	// Watchdog Timer window size is 75%
		| (0b10000 << _FWDT_SWDTPS_POSITION);	// Sleep mode Watchdog Postscale is 1:65536 

	const uint32_t __attribute__((section (".SECTION_AFOSCSEL"))) temp_afoscsel =
		0xFFFF2828
		| (0b01 << _FOSCSEL_FCKSM_POSITION)		// Clock switching ENABLED, Fail-Safe clock monitor disabled
		| (0b1 << _FOSCSEL_SOSCSEL_POSITION)	// Crystal is used for SOSC, RA4/RB4 controlled by SOSC
		| (0b1 << _FOSCSEL_OSCIOFNC_POSITION)	// System clock not on CLKO pin, operates as normal I/O
		| (0b11 << _FOSCSEL_POSCMOD_POSITION)	// Primary oscillator is disabled
		| (0b0 << _FOSCSEL_IESO_POSITION)		// Two-speed Start-up is disabled
		| (0b0 << _FOSCSEL_SOSCEN_POSITION)		// Secondary oscillator disabled
		| (0b1 << _FOSCSEL_PLLSRC_POSITION)		// FRC is input to PLL on device Reset
		| (0b001 << _FOSCSEL_FNOSC_POSITION);	// Oscillator is FRC with "Divide-by-N". We will switch to PLL later.

	const uint32_t __attribute__((section (".SECTION_AFSEC"))) temp_afsec =
		0x7FFFFFFF
		| (0b1 << _FSEC_CP_POSITION);			// Code protection disabled
	// End ALTERNATE configuration
#elif defined (__32MM0256GPM028__)
	const uint32_t __attribute__((section (".SECTION_FDEVOPT"))) temp_fdevopt = 
		0x00003FE7
		| ((0xF0C5) << _FDEVOPT_USERID_POSITION)	// UserID is F0C5. NOTE! for some reason, it's declared as _17_ bits wide in the .h file. 		
													// Hence this hack, to have it aligned, since the definition shifts by 15, instead of 16...
		| (0b0 << _FDEVOPT_FVBUSIO_POSITION)	// VBUSON pin is controlled by port function, not USB
		| (0b0 << _FDEVOPT_FUSBIDIO_POSITION)	// USBID pin is controlled by port function, not USB	
		| (0b1 << _FDEVOPT_ALTI2C_POSITION)	// SDA1 and SCL1 are on pins RB8 and RB9											
		| (0b1 << _FDEVOPT_SOSCHP_POSITION);	// SOSC operated in Normal Power mode

	const uint32_t __attribute__((section (".SECTION_FICD"))) temp_ficd =
		0xFFFFFFE3
		| (0b10 << _FICD_ICS_POSITION)			// Communication is on PGEC2/PGED2
		| (0b0 << _FICD_JTAGEN_POSITION);		// JTAG is DISABLED, otherwise pins don't work in normal use

	const uint32_t __attribute__((section (".SECTION_FPOR"))) temp_fpor =
		0xFFFFFFF0
		| (0b0 << _FPOR_LPBOREN_POSITION)		// Low-Power BOR is disabled
		| (0b0 << _FPOR_RETVR_POSITION)			// Retention regulator is enabled, controlled by RETEN in sleep
		| (0b00 << _FPOR_BOREN_POSITION);		// Brown-out reset disabled in HW, SBOREN bit disabled

	const uint32_t __attribute__((section (".SECTION_FWDT"))) temp_fwdt =
		0xFFFF0000
		| (0b0 << _FWDT_FWDTEN_POSITION)		// Watchdog Timer disabled
		| (0b00 << _FWDT_RCLKSEL_POSITION)		// Clock source is system clock
		| (0b10000 << _FWDT_RWDTPS_POSITION)	// Run mode Watchdog Postscale is 1:65536
		| (0b1 << _FWDT_WINDIS_POSITION)		// Windowed mode is disabled
		| (0b00 << _FWDT_FWDTWINSZ_POSITION)	// Watchdog Timer window size is 75%
		| (0b10000 << _FWDT_SWDTPS_POSITION);	// Sleep mode Watchdog Postscale is 1:65536 

	const uint32_t __attribute__((section (".SECTION_FOSCSEL"))) temp_foscsel =
		0xFFFF2828
		| (0b01 << _FOSCSEL_FCKSM_POSITION)		// Clock switching ENABLED, Fail-Safe clock monitor disabled
		| (0b0 << _FOSCSEL_SOSCSEL_POSITION)	// External clock connected to SOSCO, RA4/RB4 controlled by I/O PORTx
		| (0b1 << _FOSCSEL_OSCIOFNC_POSITION)	// System clock not on CLKO pin, operates as normal I/O
		| (0b11 << _FOSCSEL_POSCMOD_POSITION)	// Primary oscillator is disabled
		| (0b0 << _FOSCSEL_IESO_POSITION)		// Two-speed Start-up is disabled
		| (0b0 << _FOSCSEL_SOSCEN_POSITION)		// Secondary oscillator disabled
		| (0b1 << _FOSCSEL_PLLSRC_POSITION)		// FRC is input to PLL on device Reset
		| (0b000 << _FOSCSEL_FNOSC_POSITION);	// Oscillator is FRC with "Divide-by-N". We will switch to PLL later.

	const uint32_t __attribute__((section (".SECTION_FSEC"))) temp_fsec =
		0x7FFFFFFF
		| (0b1 << _FSEC_CP_POSITION);			// Code protection disabled
	// End PRIMARY configuration

	// Configuration bits for the PIC32MM chip. ALTERNATE configuration
	const uint32_t __attribute__((section (".SECTION_AFDEVOPT"))) temp_afdevopt = 
		0x0000FFF7
		| ((0xF0C5) << _FDEVOPT_USERID_POSITION)	// UserID is F0C5. NOTE! for some reason, it's declared as _17_ bits wide in the .h file. 
													// Hence this hack, to have it aligned, since the definition shifts by 15, instead of 16...
		| (0b1 << _FDEVOPT_SOSCHP_POSITION);	// SOSC operated in Normal Power mode

	const uint32_t __attribute__((section (".SECTION_AFICD"))) temp_aficd =
		0xFFFFFFE3
		| (0b01 << _FICD_ICS_POSITION)			// Communication is on PGEC3/PGED3
		| (0b0 << _FICD_JTAGEN_POSITION);		// JTAG is DISABLED, otherwise pins don't work in normal use

	const uint32_t __attribute__((section (".SECTION_AFPOR"))) temp_afpor =
		0xFFFFFFF0
		| (0b0 << _FPOR_LPBOREN_POSITION)		// Low-Power BOR is disabled
		| (0b0 << _FPOR_RETVR_POSITION)			// Retention regulator is enabled, controlled by RETEN in sleep
		| (0b00 << _FPOR_BOREN_POSITION);		// Brown-out reset disabled in HW, SBOREN bit disabled

	const uint32_t __attribute__((section (".SECTION_AFWDT"))) temp_afwdt =
		0xFFFF0000
		| (0b0 << _FWDT_FWDTEN_POSITION)		// Watchdog Timer disabled
		| (0b00 << _FWDT_RCLKSEL_POSITION)		// Clock source is system clock
		| (0b10000 << _FWDT_RWDTPS_POSITION)	// Run mode Watchdog Postscale is 1:65536
		| (0b1 << _FWDT_WINDIS_POSITION)		// Windowed mode is disabled
		| (0b00 << _FWDT_FWDTWINSZ_POSITION)	// Watchdog Timer window size is 75%
		| (0b10000 << _FWDT_SWDTPS_POSITION);	// Sleep mode Watchdog Postscale is 1:65536 

	const uint32_t __attribute__((section (".SECTION_AFOSCSEL"))) temp_afoscsel =
		0xFFFF2828
		| (0b01 << _FOSCSEL_FCKSM_POSITION)		// Clock switching ENABLED, Fail-Safe clock monitor disabled
		| (0b0 << _FOSCSEL_SOSCSEL_POSITION)	// External clock connected to SOSCO, RA4/RB4 controlled by I/O PORTx
		| (0b1 << _FOSCSEL_OSCIOFNC_POSITION)	// System clock not on CLKO pin, operates as normal I/O
		| (0b11 << _FOSCSEL_POSCMOD_POSITION)	// Primary oscillator is disabled
		| (0b0 << _FOSCSEL_IESO_POSITION)		// Two-speed Start-up is disabled
		| (0b0 << _FOSCSEL_SOSCEN_POSITION)		// Secondary oscillator disabled
		| (0b1 << _FOSCSEL_PLLSRC_POSITION)		// FRC is input to PLL on device Reset
		| (0b001 << _FOSCSEL_FNOSC_POSITION);	// Oscillator is FRC with "Divide-by-N". We will switch to PLL later.

	const uint32_t __attribute__((section (".SECTION_AFSEC"))) temp_afsec =
		0x7FFFFFFF
		| (0b1 << _FSEC_CP_POSITION);			// Code protection disabled
	// End ALTERNATE configuration
#endif
