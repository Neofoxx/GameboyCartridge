#include <p32xxxx.h>
#include <inttypes.h>
#include <support.h>

// There should be a 12MHz timer in the core.
// 2^32 / 12000000 = 357s max delay. We're fine.
void delayUs(uint32_t us){
	uint32_t start = _CP0_GET_COUNT();
	uint32_t toCount = (us * 12);
	while ( (_CP0_GET_COUNT() - start) < toCount){
	}
}

void delayMs(uint32_t ms){
	if (ms<357000){
		delayUs(ms*1000);
	}
	else{
		while(ms > 357000){
			delayMs(300000);
			ms = ms - 300000;
		}
		delayMs(ms);
	}
}
