#ifndef COMMANDEXECUTOR_H_bae78005cfae4d6b80ac0b49984b8bd2
#define COMMANDEXECUTOR_H_bae78005cfae4d6b80ac0b49984b8bd2

#include <inttypes.h>

void commandExecutor(char* inString);

uint8_t cartridgeReadByte(uint16_t address);
void cartridgeWriteByte(uint16_t address, uint8_t data);

#endif
