#include <forNewlib.h>
#include <errno.h>


unsigned long __stack_chk_guard;
void __stack_chk_guard_setup(void){
     __stack_chk_guard = 0xBAAAAAAD;	// Provide some magic numbers
}

void * sbrk(uint32_t extraMem){

	extern uint8_t _heap_head;	// In linker
	extern uint8_t _heap_end;	// Also in linker

	static uint8_t * heap_head = 0;
	uint8_t * old_heap_head;



	if (heap_head == 0){
		heap_head = &_heap_head;
	}

	old_heap_head = heap_head;

	if ((heap_head + extraMem) > &_heap_end){
		// FYI, code execution will be blocked if you get here.
		errno = ENOMEM;
		return (void *) -1;
	}

	heap_head = heap_head + extraMem;
	return (void *) old_heap_head;
}

