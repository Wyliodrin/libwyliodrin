#ifndef __BITS_AND_BYTES__
#define __BITS_AND_BYTES__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t lowByte(unsigned int w);

uint8_t highByte(unsigned int w);

unsigned int bitRead (unsigned int value, unsigned int bit);

unsigned int bitSet (unsigned int value, unsigned int bit);

unsigned int bitClear (unsigned int value, unsigned int bit);

unsigned int bitWrite(unsigned int value, unsigned int bit, unsigned int bitvalue);

typedef unsigned int word;

unsigned int bit(int b);

#define _BV(b) bit(b)

typedef uint8_t boolean;
typedef uint8_t byte;

#ifdef __cplusplus
}
#endif

#endif /* __BITS_AND_BYTES__ */
