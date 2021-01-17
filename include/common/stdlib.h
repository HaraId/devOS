#ifndef __DHARMA_STDLIB_H
#define __DHARMA_STDLIB_H

#include "common/types.h"

int8_t memcmp(const uint8_t* mem1, const uint8_t* mem2, const uint32_t count);

void memcpy(const uint8_t* src, uint8_t* dst, const uint32_t count);

void memsetb(uint8_t* dst, const uint8_t byte, const uint32_t count);
void memsetw(uint16_t* dst, const uint16_t word, const uint32_t count);
void memsetdw(uint32_t* dst, const uint32_t dword, const uint32_t count);


#endif