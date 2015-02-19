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
 *
 * PARAMETERS
 * 		w - two bytes integer
 *
 * RETURN
 *		low byte of <w>
 */
uint8_t lowByte(unsigned int w) 
{
  return w & 0xff;
}

/**
 * Return the high byte from <w>
 *
 * PARAMETERS
 * 		w - two bytes integer
 *
 * RETURN
 *		high byte of <w>
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
 *
 * PARAMETERS
 *		b - number of positions
 *
 * RETURN
 *		2 raised to the power <b>
 */
unsigned int bit(int b)
{
  return (1UL << (b));
}

/**
 * Return the bit number <bit> from <value>
 *
 * PARAMETERS
 * 		value - unsigned integer
 *		bit   - bit position to be read
 *
 * RETURN
 *    either 0 or 1
 */
unsigned int bitRead(unsigned int value, unsigned int bit) 
{
  return (((value) >> (bit)) & 0x01);
}

/**
 * Set the bit number <bit> to 1 in <value>
 *
 * PARAMETERS
 *		value - unsigned integer
 *		bit   - bit position
 *
 * RETURN
 *		<value> with the bit at position <bit> set to 1
 */
unsigned int bitSet(unsigned int value, unsigned int bit) 
{
  return ((value) |= (1UL << (bit)));
}

/**
 * Set the bit number <bit> to 0 in <value>
 *
 * PARAMETERS
 *		value - unsigned integer
 *		bit   - bit position
 *
 * RETURN
 *		<value> with the bit at position <bit> set to 0
 */
unsigned int bitClear(unsigned int value, unsigned int bit)
{
  return ((value) &= ~(1UL << (bit)));
}

/**
 * Set the bit number <bit> to <bitvalue> in <value>
 *
 * PARAMETERS
 *		value    - unsigned integer
 *		bit      - bit position
 *    bitvalue - either 0 or 1
 *
 * RETURN
 *		<value> with the bit at position <bit> set to <bitvalue>
 */
unsigned int bitWrite(unsigned int value, unsigned int bit, unsigned int bitvalue)
{
  return (bitvalue ? bitSet(value, bit) : bitClear(value, bit));
}



#ifdef __cplusplus
}
#endif
