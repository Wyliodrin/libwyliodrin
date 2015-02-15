/**************************************************************************************************
 * Bits and bytes handling.
 *
 * CONTENT
 * 1. Bytes
 * 2. Bits
 *************************************************************************************************/

#ifndef BITS_AND_BYTES_H
#define BITS_AND_BYTES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _BV(b) bit(b)

typedef unsigned int word;
typedef uint8_t boolean;
typedef uint8_t byte;



/**************************************************************************************************
 * 1. Bytes
 *************************************************************************************************/

uint8_t lowByte  (unsigned int w);
uint8_t highByte (unsigned int w);



/**************************************************************************************************
 * 2. Bits
 *************************************************************************************************/

unsigned int bit      (int b);
unsigned int bitRead  (unsigned int value, unsigned int bit);
unsigned int bitSet   (unsigned int value, unsigned int bit);
unsigned int bitClear (unsigned int value, unsigned int bit);
unsigned int bitWrite (unsigned int value, unsigned int bit, unsigned int bitvalue);



#ifdef __cplusplus
}
#endif

#endif // BITS_AND_BYTES_H
