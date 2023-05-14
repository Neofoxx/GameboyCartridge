/*
 * USB Descriptors file
 *
 * This file may be used by anyone for any purpose and may be used as a
 * starting point making your own application using M-Stack.
 *
 * It is worth noting that M-Stack itself is not under the same license as
 * this file.
 *
 * M-Stack is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  For details, see sections 7, 8, and 9
 * of the Apache License, version 2.0 which apply to this file.  If you have
 * purchased a commercial license for this software from Signal 11 Software,
 * your commerical license superceeds the information in this header.
 *
 * Alan Ott
 * Signal 11 Software
 */

#include "usb_config.h"
#include "usb.h"
#include "usb_ch9.h"
#include "usb_cdc.h"

#define ROMPTR

/* Configuration Packet
 *
 * This packet contains a configuration descriptor, one or more interface
 * descriptors, class descriptors(optional), and endpoint descriptors for a
 * single configuration of the device.  This struct is specific to the
 * device, so the application will need to add any interfaces, classes and
 * endpoints it intends to use.  It is sent to the host in response to a
 * GET_DESCRIPTOR[CONFIGURATION] request.
 *
 * While Most devices will only have one configuration, a device can have as
 * many configurations as it needs.  To have more than one, simply make as
 * many of these structs as are required, one for each configuration.
 *
 * An instance of each configuration packet must be put in the
 * usb_application_config_descs[] array below (which is #defined in
 * usb_config.h) so that the USB stack can find it.
 *
 * See Chapter 9 of the USB specification from usb.org for details.
 *
 * It's worth noting that adding endpoints here does not automatically
 * enable them in the USB stack.  To use an endpoint, it must be declared
 * here and also in usb_config.h.
 *
 * The configuration packet below is for the mouse demo application.
 * Yours will of course vary.
 */
struct configuration_1_packet {
	struct configuration_descriptor  config;

	// FIRST CDC interface - programmer
	struct interface_association_descriptor iad_first;

	/* CDC Class Interface */
	struct interface_descriptor      cdc_class_interface_first;
	struct cdc_functional_descriptor_header cdc_func_header_first;
	struct cdc_acm_functional_descriptor cdc_acm_first;
	struct cdc_union_functional_descriptor cdc_union_first;
	struct endpoint_descriptor       cdc_ep_first;

	/* CDC Data Interface */
	struct interface_descriptor      cdc_data_interface_first;
	struct endpoint_descriptor       data_ep_in_first;
	struct endpoint_descriptor       data_ep_out_first;



	// // SECOND CDC interface - USB-UART converter
	// struct interface_association_descriptor iad_second;

	// /* CDC Class Interface */
	// struct interface_descriptor      cdc_class_interface_second;
	// struct cdc_functional_descriptor_header cdc_func_header_second;
	// struct cdc_acm_functional_descriptor cdc_acm_second;
	// struct cdc_union_functional_descriptor cdc_union_second;
	// struct endpoint_descriptor       cdc_ep_second;

	// /* CDC Data Interface */
	// struct interface_descriptor      cdc_data_interface_second;
	// struct endpoint_descriptor       data_ep_in_second;
	// struct endpoint_descriptor       data_ep_out_second;



};


/* Device Descriptor
 *
 * Each device has a single device descriptor describing the device.  The
 * format is described in Chapter 9 of the USB specification from usb.org.
 * USB_DEVICE_DESCRIPTOR needs to be defined to the name of this object in
 * usb_config.h.  For more information, see USB_DEVICE_DESCRIPTOR in usb.h.
 */
const ROMPTR struct device_descriptor this_device_descriptor =
{
	sizeof(struct device_descriptor), // bLength
	DESC_DEVICE, 							// bDescriptorType
	0x0200, 								// 0x0200 = USB 2.0, 0x0110 = USB 1.1
	DEVICE_CLASS_MISC, 						// Device class
	0x02, 									/* Device Subclass. See the document entitled: "USB Interface
												 Association Descriptor Device Class Code and Use Model" */
											// Set to 0x02. If both class and subclass are 0x02, it'll be handled as a serial device
											// automatically in Windows - https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/usb-driver-installation-based-on-compatible-ids
											// TODO CHECK!
	0x01, 									// Protocol. Unsure what to set
	EP_0_LEN, 								// bMaxPacketSize0 - Could be bigger? Check.
	0x1209, 								// VID - Vendor ID, from pid.codes
	0x4E52, 								// PID - Product - "NR" - not set in stone, need to submit to pid.codes for approval.
	0x0001, // device release (1.0)
	1, 										// Index of Manufacturer String Descriptor
	2, 										// Index of Product String Descriptor
	5, 										// Index of Serial String Descriptor
	NUMBER_OF_CONFIGURATIONS // NumConfigurations
};

/* Configuration Packet Instance
 *
 * This is an instance of the configuration_packet struct containing all the
 * data describing a single configuration of this device.  It is wise to use
 * as much C here as possible, such as sizeof() operators, and #defines from
 * usb_config.h.  When stuff is wrong here, it can be difficult to track
 * down exactly why, so it's good to get the compiler to do as much of it
 * for you as it can.
 */
static const ROMPTR struct configuration_1_packet configuration_1 =
{
	{
		// Members from struct configuration_descriptor
		sizeof(struct configuration_descriptor),
		DESC_CONFIGURATION,
		sizeof(configuration_1), // wTotalLength (length of the whole packet)
		// 4, // bNumInterfaces	- changed
		2, // bNumInterfaces
		1, // bConfigurationValue
		2, // iConfiguration (index of string descriptor)
		0b10000000,
		100/2,   // 100/2 indicates 100mA
	},

	////////////////////////////////////////////////////////////////////
	// FIRST CDC INTERFACE - programmer
	////////////////////////////////////////////////////////////////////
	/* Interface Association Descriptor */
	{
		sizeof(struct interface_association_descriptor),
		DESC_INTERFACE_ASSOCIATION,
		0, /* bFirstInterface */
		2, /* bInterfaceCount */
		CDC_COMMUNICATION_INTERFACE_CLASS,
		CDC_COMMUNICATION_INTERFACE_CLASS_ACM_SUBCLASS,
		0, /* bFunctionProtocol */
		2, /* iFunction (string descriptor index) */
	},

	/* CDC Class Interface */
	{
		// Members from struct interface_descriptor
		sizeof(struct interface_descriptor), // bLength;
		DESC_INTERFACE,
		0x0, // InterfaceNumber
		0x0, // AlternateSetting
		0x1, // bNumEndpoints
		CDC_COMMUNICATION_INTERFACE_CLASS, // bInterfaceClass
		CDC_COMMUNICATION_INTERFACE_CLASS_ACM_SUBCLASS, // bInterfaceSubclass
		0x00, // bInterfaceProtocol
		0x03, // iInterface (index of string describing interface)
	},

	/* CDC Functional Descriptor Header */
	{
		sizeof(struct cdc_functional_descriptor_header),
		DESC_CS_INTERFACE,
		CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_HEADER,
		0x0110, /* bcdCDC (version in BCD) */
	},

	/* CDC ACM Functional Descriptor */
	{
		sizeof(struct cdc_acm_functional_descriptor),
		DESC_CS_INTERFACE,
		CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_ACM,
		/* bmCapabilities: Make sure to keep in sync with the actual
		* capabilities (ie: which callbacks are defined). */
		CDC_ACM_CAPABILITY_LINE_CODINGS | CDC_ACM_CAPABILITY_SEND_BREAK,
	},

	/* CDC Union Functional Descriptor */
	{
		sizeof (struct cdc_union_functional_descriptor),
		DESC_CS_INTERFACE,
		CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_UNION,
		0, /* bMasterInterface */
		1, /* bSlaveInterface0 */
	},

	/* CDC ACM Notification Endpoint (Endpoint 1 IN) */
	{
		sizeof(struct endpoint_descriptor),
		DESC_ENDPOINT,
		0x01 | 0x80, // endpoint #1 0x80=IN
		EP_INTERRUPT, // bmAttributes
		EP_1_IN_LEN, // wMaxPacketSize
		1, // bInterval in ms.
	},

	/* CDC Data Interface */
	{
	// Members from struct interface_descriptor
		sizeof(struct interface_descriptor), // bLength;
		DESC_INTERFACE,
		0x1, // InterfaceNumber
		0x0, // AlternateSetting
		0x2, // bNumEndpoints
		CDC_DATA_INTERFACE_CLASS, // bInterfaceClass
		0, // bInterfaceSubclass (no subclass)
		CDC_DATA_INTERFACE_CLASS_PROTOCOL_NONE, // bInterfaceProtocol
		0x04, // iInterface (index of string describing interface)
	},

	/* CDC Data IN Endpoint */
	{
		sizeof(struct endpoint_descriptor),
		DESC_ENDPOINT,
		0x02 | 0x80, // endpoint #2 0x80=IN
		EP_BULK, // bmAttributes
		EP_2_IN_LEN, // wMaxPacketSize
		1, // bInterval in ms.
	},

	/* CDC Data OUT Endpoint */
	{
		sizeof(struct endpoint_descriptor),
		DESC_ENDPOINT,
		0x02 /*| 0x00*/, // endpoint #2 0x00=OUT
		EP_BULK, // bmAttributes
		EP_2_OUT_LEN, // wMaxPacketSize
		1, // bInterval in ms.
	},


	////////////////////////////////////////////////////////////////////
	// FIRST CDC INTERFACE - programmer
	////////////////////////////////////////////////////////////////////
	/* Interface Association Descriptor */
	// {
	// sizeof(struct interface_association_descriptor),
	// DESC_INTERFACE_ASSOCIATION,
	// 2, /* bFirstInterface */	// Set to interface 2, since First IAD is interface 0 and goes for 2 interfaces
	// 2, /* bInterfaceCount */
	// CDC_COMMUNICATION_INTERFACE_CLASS,
	// CDC_COMMUNICATION_INTERFACE_CLASS_ACM_SUBCLASS,
	// 0, /* bFunctionProtocol */
	// 2, /* iFunction (string descriptor index) */	// TODO CHECK
	// },

	// /* CDC Class Interface */
	// {
	// // Members from struct interface_descriptor
	// sizeof(struct interface_descriptor), // bLength;
	// DESC_INTERFACE,
	// 0x2, // InterfaceNumber		// Set to interface 2
	// 0x0, // AlternateSetting
	// 0x1, // bNumEndpoints		// 1 endpoint - The INTERRUPT endpoint that has to be for every CDC device.
	// CDC_COMMUNICATION_INTERFACE_CLASS, // bInterfaceClass
	// CDC_COMMUNICATION_INTERFACE_CLASS_ACM_SUBCLASS, // bInterfaceSubclass
	// 0x00, // bInterfaceProtocol
	// 0x06, // iInterface (index of string describing interface) - changed from 3 to 6
	// },

	// /* CDC Functional Descriptor Header */
	// {
	// sizeof(struct cdc_functional_descriptor_header),
	// DESC_CS_INTERFACE,
	// CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_HEADER,
	// 0x0110, /* bcdCDC (version in BCD) */
	// },

	// /* CDC ACM Functional Descriptor */
	// {
	// sizeof(struct cdc_acm_functional_descriptor),
	// DESC_CS_INTERFACE,
	// CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_ACM,
	// /* bmCapabilities: Make sure to keep in sync with the actual
	//  * capabilities (ie: which callbacks are defined). */
	// CDC_ACM_CAPABILITY_LINE_CODINGS | CDC_ACM_CAPABILITY_SEND_BREAK,
	// },

	// /* CDC Union Functional Descriptor */
	// {
	// sizeof (struct cdc_union_functional_descriptor),
	// DESC_CS_INTERFACE,
	// CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_UNION,
	// 2, /* bMasterInterface */ // CHANGED
	// 3, /* bSlaveInterface0 */ // CHANGED
	// },

	// /* CDC ACM Notification Endpoint (Endpoint 1 IN) */
	// {
	// sizeof(struct endpoint_descriptor),
	// DESC_ENDPOINT,
	// 0x03 | 0x80, // endpoint #3 0x80=IN - updated
	// EP_INTERRUPT, // bmAttributes
	// EP_3_IN_LEN, // wMaxPacketSize
	// 1, // bInterval in ms.
	// },

	// /* CDC Data Interface */
	// {
	// // Members from struct interface_descriptor
	// sizeof(struct interface_descriptor), // bLength;
	// DESC_INTERFACE,
	// 0x3, // InterfaceNumber
	// 0x0, // AlternateSetting
	// 0x2, // bNumEndpoints
	// CDC_DATA_INTERFACE_CLASS, // bInterfaceClass
	// 0, // bInterfaceSubclass (no subclass)
	// CDC_DATA_INTERFACE_CLASS_PROTOCOL_NONE, // bInterfaceProtocol
	// 0x07, // iInterface (index of string describing interface) - changed from 4 to 7
	// },

	// /* CDC Data IN Endpoint */
	// {
	// sizeof(struct endpoint_descriptor),
	// DESC_ENDPOINT,
	// 0x04 | 0x80, // endpoint #4 0x80=IN - updated
	// EP_BULK, // bmAttributes
	// EP_4_IN_LEN, // wMaxPacketSize
	// 1, // bInterval in ms.
	// },

	// /* CDC Data OUT Endpoint */
	// {
	// sizeof(struct endpoint_descriptor),
	// DESC_ENDPOINT,
	// 0x04 /*| 0x00*/, // endpoint #4 0x00=OUT - updated
	// EP_BULK, // bmAttributes
	// EP_4_OUT_LEN, // wMaxPacketSize
	// 1, // bInterval in ms.
	// },


};

/* String Descriptors
 *
 * String descriptors are optional. If strings are used, string #0 is
 * required, and must contain the language ID of the other strings.  See
 * Chapter 9 of the USB specification from usb.org for more info.
 *
 * Strings are UTF-16 Unicode, and are not NULL-terminated, hence the
 * unusual syntax.
 */

/* String index 0, only has one character in it, which is to be set to the
   language ID of the language which the other strings are in. */
static const ROMPTR struct {uint8_t bLength;uint8_t bDescriptorType; uint16_t lang; } str00 = {
	sizeof(str00),
	DESC_STRING,
	0x0409 // US English
};

static const ROMPTR struct {uint8_t bLength;uint8_t bDescriptorType; uint16_t chars[7]; } vendor_string = {
	sizeof(vendor_string),
	DESC_STRING,
	{'N', 'e', 'o', 'f', 'o', 'x', 'x'}
};

static const ROMPTR struct {uint8_t bLength;uint8_t bDescriptorType; uint16_t chars[22]; } product_string = {
	sizeof(product_string),
	DESC_STRING,
	{'3', '2', 'k', 'B', ' ', 'G', 'a', 'm', 'e', 'b', 'o', 'y', ' ', 'C', 'a', 'r', 't', 'r', 'i', 'd', 'g', 'e'}
};

static const ROMPTR struct {uint8_t bLength;uint8_t bDescriptorType; uint16_t chars[14]; } cdc_interface_string_first = {
	sizeof(cdc_interface_string_first),
	DESC_STRING,
	{'P','R','O','G',' ','I','n','t','e','r','f','a','c','e'}
};

static const ROMPTR struct {uint8_t bLength;uint8_t bDescriptorType; uint16_t chars[19]; } cdc_data_string_first = {
	sizeof(cdc_data_string_first),
	DESC_STRING,
	{'P','R','O','G',' ','D','a','t','a',' ','I','n','t','e','r','f','a','c','e'}
};

static const ROMPTR struct {uint8_t bLength;uint8_t bDescriptorType; uint16_t chars[10]; } fake_serial_num = {
	sizeof(fake_serial_num),
	DESC_STRING,
	{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9' }
};


/* Get String function
 *
 * This function is called by the USB stack to get a pointer to a string
 * descriptor.  If using strings, USB_STRING_DESCRIPTOR_FUNC must be defined
 * to the name of this function in usb_config.h.  See
 * USB_STRING_DESCRIPTOR_FUNC in usb.h for information about this function.
 * This is a function, and not simply a list or map, because it is useful,
 * and advisable, to have a serial number string which may be read from
 * EEPROM or somewhere that's not part of static program memory.
 */
int16_t usb_application_get_string(uint8_t string_number, const void **ptr)
{
	if (string_number == 0) {
		*ptr = &str00;
		return sizeof(str00);
	}
	else if (string_number == 1) {
		*ptr = &vendor_string;
		return sizeof(vendor_string);
	}
	else if (string_number == 2) {
		*ptr = &product_string;
		return sizeof(product_string);
	}
	else if (string_number == 3) {			// For FIRST CDC interface
		*ptr = &cdc_interface_string_first;
		return sizeof(cdc_interface_string_first);
	}
	else if (string_number == 4) {			// For FIRST CDC interface
		*ptr = &cdc_data_string_first;
		return sizeof(cdc_data_string_first);
	}
	else if (string_number == 5) {
		/* This is where you will have code to do something like read
		 * a serial number out of EEPROM and return it. For CDC
		 * devices, this is a MUST.
		 *
		 * However, since this is a demo, we will return a fake,
		 * hard-coded serial number here. PLEASE don't ship products
		 * like this. If you do, your customers will be mad as soon
		 * as they plug two of your devices in at the same time. */

		// On PIC32MX, there isn't anywhere to get any sort of UUID...
		// Keep the fake serial for now.
		*ptr = &fake_serial_num;
		return sizeof(fake_serial_num);
	}
	// else if (string_number == 6) {			// For SECOND CDC interface
	// 	*ptr = &cdc_interface_string_second;
	// 	return sizeof(cdc_interface_string_second);
	// }
	// else if (string_number == 7) {			// For SECOND CDC interface
	// 	*ptr = &cdc_data_string_second;
	// 	return sizeof(cdc_data_string_second);
	// }

	return -1;
}

/* Configuration Descriptor List
 *
 * This is the list of pointters to the device's configuration descriptors.
 * The USB stack will read this array looking for descriptors which are
 * requsted from the host.  USB_CONFIG_DESCRIPTOR_MAP must be defined to the
 * name of this array in usb_config.h.  See USB_CONFIG_DESCRIPTOR_MAP in
 * usb.h for information about this array.  The order of the descriptors is
 * not important, as the USB stack reads bConfigurationValue for each
 * descriptor to know its index.  Make sure NUMBER_OF_CONFIGURATIONS in
 * usb_config.h matches the number of descriptors in this array.
 */
const struct configuration_descriptor *usb_application_config_descs[] =
{
	(struct configuration_descriptor*) &configuration_1,
};
STATIC_SIZE_CHECK_EQUAL(USB_ARRAYLEN(USB_CONFIG_DESCRIPTOR_MAP), NUMBER_OF_CONFIGURATIONS);
STATIC_SIZE_CHECK_EQUAL(sizeof(USB_DEVICE_DESCRIPTOR), 18);

