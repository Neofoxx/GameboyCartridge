/*
 *  M-Stack Hardware Abstraction
 *  Copyright (C) 2013 Alan Ott <alan@signal11.us>
 *  Copyright (C) 2013 Signal 11 Software
 *
 *  2013-04-08
 *
 *  M-Stack is free software: you can redistribute it and/or modify it under
 *  the terms of the GNU Lesser General Public License as published by the
 *  Free Software Foundation, version 3; or the Apache License, version 2.0
 *  as published by the Apache Software Foundation.  If you have purchased a
 *  commercial license for this software from Signal 11 Software, your
 *  commerical license superceeds the information in this header.
 *
 *  M-Stack is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this software.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  You should have received a copy of the Apache License, verion 2.0 along
 *  with this software.  If not, see <http://www.apache.org/licenses/>.
 */

#ifndef USB_HAL_H__
#define UAB_HAL_H__


#define USB_NEEDS_POWER_ON
#define USB_NEEDS_SET_BD_ADDR_REG
#define USB_FULL_PING_PONG_ONLY

#define BDNADR_TYPE              uint32_t /* physical address */
#define PHYS_ADDR(VIRTUAL_ADDR)  KVA_TO_PA(VIRTUAL_ADDR)

#define SFR_PULL_EN              /* Not used on PIC32MX */
#define SFR_ON_CHIP_XCVR_DIS     U1CNFG2bits.UTRDIS
#define SET_PING_PONG_MODE(n)    U1CNFG1bits.PPB = n

#define SFR_USB_INTERRUPT_FLAGS  U1IR
#define SFR_USB_RESET_IF         U1IRbits.URSTIF
#define SFR_USB_STALL_IF         U1IRbits.STALLIF
#define SFR_USB_TOKEN_IF         U1IRbits.TRNIF
#define SFR_USB_SOF_IF           U1IRbits.SOFIF
#ifdef __PIC32MX__
	#define SFR_USB_IF               IFS1bits.USBIF
#elif __PIC32MM__
	#define SFR_USB_IF               IFS0bits.USBIF
#endif


#define SFR_USB_INTERRUPT_EN     U1IE
#define SFR_TRANSFER_IE          U1IEbits.TRNIE
#define SFR_STALL_IE             U1IEbits.STALLIE
#define SFR_RESET_IE             U1IEbits.URSTIE
#define SFR_SOF_IE               U1IEbits.SOFIE
#ifdef __PIC32MX__
	#define SFR_USB_IE               IEC1bits.USBIE
#elif __PIC32MM__
	#define SFR_USB_IE               IEC0bits.USBIE
#endif

#define SFR_USB_EXTENDED_INTERRUPT_EN U1EIE

#define SFR_EP_MGMT_TYPE         __U1EP1bits_t
#define UEP_REG_STRIDE           4
#define SFR_EP_MGMT(ep)          ((SFR_EP_MGMT_TYPE*) (&U1EP0 + UEP_REG_STRIDE * (ep)))
#define SFR_EP_MGMT_HANDSHAKE    EPHSHK
#define SFR_EP_MGMT_STALL        EPSTALL
#define SFR_EP_MGMT_IN_EN        EPTXEN   /* In/out from HOST perspective */
#define SFR_EP_MGMT_OUT_EN       EPRXEN
#define SFR_EP_MGMT_CON_DIS      EPCONDIS /* disable control transfers */
                                 /* Ignoring RETRYDIS and LSPD for now */
#define SFR_USB_ADDR             U1ADDR
#define SFR_USB_EN               U1CONbits.USBEN
#define SFR_USB_PKT_DIS          U1CONbits.PKTDIS
#define SFR_USB_PING_PONG_RESET  U1CONbits.PPBRST


#define SFR_USB_STATUS           U1STAT
#define SFR_USB_STATUS_EP        U1STATbits.ENDPT
#define SFR_USB_STATUS_DIR       U1STATbits.DIR
#define SFR_USB_STATUS_PPBI      U1STATbits.PPBI

#define SFR_USB_POWER            U1PWRCbits.USBPWR
#define SFR_BD_ADDR_REG1         U1BDTP1
#define SFR_BD_ADDR_REG2         U1BDTP2
#define SFR_BD_ADDR_REG3         U1BDTP3

#define SFR_OTGEN                U1OTGCONbits.OTGEN
#define SFR_DPPULUP              U1OTGCONbits.DPPULUP

#define CLEAR_ALL_USB_IF()       do { SFR_USB_INTERRUPT_FLAGS = 0xff; U1EIR = 0xff; } while(0)
#define CLEAR_USB_RESET_IF()     SFR_USB_INTERRUPT_FLAGS = 0x1
#define CLEAR_USB_STALL_IF()     SFR_USB_INTERRUPT_FLAGS = 0x80
#define CLEAR_USB_TOKEN_IF()     SFR_USB_INTERRUPT_FLAGS = 0x08
#define CLEAR_USB_SOF_IF()       SFR_USB_INTERRUPT_FLAGS = 0x4

#define BDNSTAT_UOWN   0x0080
#define BDNSTAT_DTS    0x0040
#define BDNSTAT_DTSEN  0x0008
#define BDNSTAT_BSTALL 0x0004

/* Buffer Descriptor
 *
 * This struct represents BDnSTAT in the datasheet. See the comment in the
 * 8-bit section above for more information on buffer descriptors.
 */
struct buffer_descriptor {
	union {
		struct {
			/* When receiving from the SIE. (USB Mode) */
			uint32_t : 2;
			uint32_t PID : 4;	/* See enum PID */
			uint32_t DTS: 1;	/* DATA0/1 in datasheet */
			uint32_t UOWN : 1;

			uint32_t : 8;
			uint32_t BDnCNT : 10;
			uint32_t : 6;

		};
		struct {
			/* When giving to the SIE (CPU Mode) */
			uint32_t : 2;
			uint32_t BSTALL : 1;
			uint32_t DTSEN : 1;    /* DTS in datasheet */
			uint32_t reserved : 2; /* NINC, KEEP */
			//uint32_t DTS : 1;		// Removed, since dual declarations not allowed - it's in the same place anyway.
			uint32_t DUMM : 1;	// Replacement for above.
			uint32_t /*UOWN*/ : 1;

			uint32_t : 8;
			uint32_t /*BDnCNT*/ : 10;
			uint32_t :6;
		};
		struct {
			uint16_t : 16;
			uint16_t BDnSTAT; /* Low word, where the flags are */
		};
		uint32_t BDnSTAT_CNT; /* BDnSTAT and BDnCNT as a 16-bit */
	}STAT;
	BDNADR_TYPE BDnADR;
};

#define SET_BDN(REG, FLAGS, CNT) \
                     do { (REG).STAT.BDnSTAT_CNT = (FLAGS) | ((CNT)<<16); } while(0)

#ifdef LARGE_EP
	#define BDN_LENGTH(REG) (REG.STAT.BDnCNT)
#else
	#define BDN_LENGTH(REG) (REG.STAT.BDnCNT)
#endif


#define BD_ADDR
#define BUFFER_ADDR
#define BD_ATTR_TAG __attribute__((aligned(512), coherent))

#define PPB_NONE         0 /* Unused on PIC32 */
#define PPB_EPO_OUT_ONLY 1 /* Unused on PIC32 */
#define PPB_ALL          2 /* Unused on PIC32 */
#define PPB_EPN_ONLY     3

/* Compiler stuff. Probably should be somewhere else. */
#define FAR
#define memcpy_from_rom(x,y,z) memcpy(x,y,z)



#endif /* USB_HAL_H__ */
