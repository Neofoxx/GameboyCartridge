#ifndef CONFIGBITS_H_0ec99d849f5f47efa0c4ec428b5e7358
#define CONFIGBITS_H_0ec99d849f5f47efa0c4ec428b5e7358

#include <inttypes.h>

// Config bits, defined for different chips.
// Currently for a PIC32MX270 target, and a PIC32MX440 target

#if defined(__32MX270F256D__) || defined(__32MX440F256H__)
extern const uint32_t temp3;
extern const uint32_t temp2;
extern const uint32_t temp1;
extern const uint32_t temp0;
#endif


// Expanding to more than just configuration bits.
// -> Definitions for string descriptions

typedef struct infoStruct{
	char* mcu;
	char* mode;
	char* name;
	char* revision;

} infoStruct;

extern const infoStruct info;

#endif
