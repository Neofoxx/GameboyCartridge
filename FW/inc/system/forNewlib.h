#ifndef FORNEWLIB_H_29c7660719184f48805c873b50a8ef54
#define FORNEWLIB_H_29c7660719184f48805c873b50a8ef54

#include <inttypes.h>

void __stack_chk_guard_setup(void);
void * sbrk(uint32_t extraMem);

#endif
