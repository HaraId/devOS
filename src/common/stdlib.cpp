#include "common/stdlib.h"

int8_t memcmp(const uint8_t* mem1, const uint8_t* mem2, const uint32_t count)
{
	uint32_t i = 0;

	for (i = 0; i < count; ++i)
	if ( mem1 [i] != mem2[i] )
		return (mem1[i] > mem2[i] ? 1 : -1);

	return 0;
}

void memcpy(const uint8_t* src, uint8_t* dst, const uint32_t count)
{
	uint32_t i = 0;

	for (i = 0; i < count; ++i)
		dst[i] = src[i];
}

void memset(int8_t* dst, const int8_t byte, const uint32_t count)
{
	uint32_t i = 0;

	for (i = 0; i < count; ++i)
		dst[i] = byte;
}

void memsetw(uint16_t* dst, const uint16_t word, const uint32_t count)
{
	uint32_t i = 0;

	for (i = 0; i < count; ++i)
		dst[i] = word;
}

void memsetdw(uint32_t* dst, const uint32_t dword, const uint32_t count)
{
	uint32_t i = 0;

	for (i = 0; i < count; ++i)
		dst[i] = dword;
}