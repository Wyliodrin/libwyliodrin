/**************************************************************************************************
 * Bits and bytes handling.
 *
 * CONTENT
 * 1. Bytes
 * 2. Bits
 *************************************************************************************************/

#include "bitsAndBytes.h"

#ifdef __cplusplus
extern "C" {
#endif



/**************************************************************************************************
 * 1. Bytes
 *************************************************************************************************/

/**
 * Return the low byte from <w>
 */
uint8_t lowByte(unsigned int w) 
{
  return w & 0xff;
}

/**
 * Return the high byte from <w>
 */
uint8_t highByte(unsigned int w)
{
  return w >> 8;
}



/**************************************************************************************************
 * 2. Bits
 *************************************************************************************************/

/**
 * Return an integer where the only 1 bit is on position <b>
 */
unsigned int bit(int b)
{
  return (1UL << (b));
}

/**
 * Return the bit number <bit> from <value>
 */
unsigned int bitRead(unsigned int value, unsigned int bit) 
{
  return (((value) >> (bit)) & 0x01);
}

/**
 * Set the bit number <bit> to 1 in <value>
 */
unsigned int bitSet (unsigned int value, unsigned int bit) 
{
  return ((value) |= (1UL << (bit)));
}

/**
 * Set the bit number <bit> to 0 in <value>
 */
unsigned int bitClear (unsigned int value, unsigned int bit)
{
  return ((value) &= ~(1UL << (bit)));
}

/**
 * Set the bit number <bit> to <bitvalue> in <value>
 */
unsigned int bitWrite(unsigned int value, unsigned int bit, unsigned int bitvalue)
{
  return (bitvalue ? bitSet(value, bit) : bitClear(value, bit));
}



#ifdef __cplusplus
}
#endif
