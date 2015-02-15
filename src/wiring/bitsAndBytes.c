
#include "BitsAndBytes.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t lowByte(unsigned int w) 
{
	return w & 0xff;
}

uint8_t highByte(unsigned int w)
{
	return w >> 8;
}

unsigned int bitRead (unsigned int value, unsigned int bit) 
{
	return (((value) >> (bit)) & 0x01);
}

unsigned int bitSet (unsigned int value, unsigned int bit) 
{
	return ((value) |= (1UL << (bit)));
}

unsigned int bitClear (unsigned int value, unsigned int bit)
{
	return ((value) &= ~(1UL << (bit)));
}

unsigned int bitWrite(unsigned int value, unsigned int bit, unsigned int bitvalue)
{
	return (bitvalue ? bitSet(value, bit) : bitClear(value, bit));
}

unsigned int bit(int b)
{
	return (1UL << (b));
}

#ifdef __cplusplus
}
#endif
