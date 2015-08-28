/**************************************************************************************************
 * Author: Ioana CULIC <ioana.culic@wyliodrin.com>
 * 
 * This file contains the definitions of all wiring functions for the Red Pitaya.
 *
 * CONTENT
 * 1.General
 * 2.Digital I/O
 * 3.Analog I/O
 * 4.Advanced I/O
 * 5.Time
 * 6.SPI
 * 7.I2C
 * 8.Serial
 *************************************************************************************************/

 #ifdef REDPITAYA

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rp.h>
#include "wiring.h"

rp_dpin_t gpio[] = {RP_DIO0_P, RP_DIO1_P, RP_DIO2_P, RP_DIO3_P, RP_DIO4_P, RP_DIO5_P, RP_DIO6_P, RP_DIO7_P, RP_DIO0_N, RP_DIO1_N, RP_DIO2_N, RP_DIO3_N, RP_DIO4_N, RP_DIO5_N, RP_DIO6_N, RP_DIO7_N, RP_LED0, RP_LED1, RP_LED2, RP_LED3, RP_LED4, RP_LED5, RP_LED6, RP_LED7};

rp_apin_t ain[] = {RP_AIN0, RP_AIN1, RP_AIN2, RP_AIN3};
rp_apin_t aout[] = {RP_AOUT0, RP_AOUT1, RP_AOUT2, RP_AOUT3};

#define GPIO	24
#define AIN	4
#define AOUT	4
#define AWRITE_MAX_VALUE	255
#define AREAD_MAX_VALUE		1023
#define RP_AWRITE_MAX_VALUE	156
#define RP_AREAD_MAX_VALUE	2047

/**************************************************************************************************
 * 1.General
 *************************************************************************************************/

int wiringSetup()
{
	int rc = rp_Init();
	if (rc == RP_OK)
  		return 0;
  	printf("%s\n", rp_GetError(rc));
}

void pinReset (int pin)
{
	//not implemented
	printf("%s function not implemented\n",__func__);
}

/**************************************************************************************************
 * Digital pins
 *************************************************************************************************/

void pinMode(int pin, int mode)
{
	if(pin<GPIO)
	{
		int rc = rp_DpinSetDirection(gpio[pin], mode);
		if(rc != RP_OK)
			printf("%s\n", rp_GetError(rc));
	}
	else
		printf("Cannot set pin mode for pin %d\n", pin);
}

void digitalWrite (int pin, int value)
{
	if(pin<GPIO)
	{
		pinMode (pin, OUTPUT);
		int rc = rp_DpinSetState(gpio[pin], value);
		if(rc != RP_OK)
			printf("%s\n", rp_GetError(rc));
	}
	else
		printf("Pin %d is not a GPIO pin\n", pin);
}

int digitalRead (int pin)
{
	if(pin<GPIO)
	{
		pinMode (pin, INPUT);
		rp_pinState_t value;
		int rc = rp_DpinGetState(gpio[pin], &value);
		if(rc == RP_OK)
			return value;
		printf("%s\n", rp_GetError(rc));
	}
	else
		printf("Pin %d is not a GPIO pin\n",pin);
	return -1;
}

/**************************************************************************************************
 * Analog pins
 *************************************************************************************************/

void analogWrite (int pin, int value)
{
	//Scale values from 0-1023 to 0-2047
	int scaled_value = value*RP_AWRITE_MAX_VALUE/AWRITE_MAX_VALUE;
	if(pin<AOUT)
	{
		int rc = rp_ApinSetValueRaw(aout[pin], scaled_value);
		if(rc != RP_OK)
			printf("%s\n", rp_GetError(rc));
	}
	else
		printf("Pin %d is not analog output\n",pin);
}	

void analogWriteRaw (int pin, int value)
{
	if(pin<AOUT)
	{
		int rc = rp_ApinSetValueRaw(aout[pin], value);
		if(rc != RP_OK)
			printf("%s\n", rp_GetError(rc));
	}
	else
		printf("Pin %d is not analog output\n",pin);
}

void analogWriteVoltage (int pin, float value)
{
	if(pin<AOUT)
	{
		int rc = rp_ApinSetValue(aout[pin], value);
		if(rc != RP_OK)
			printf("%s\n", rp_GetError(rc));
	}
	else
		printf("Pin %d is not analog output\n",pin);
}

int analogRead (int pin)
{
	int scaled_value;
	if(pin<AIN)
	{
		int value;
		int rc = rp_ApinGetValueRaw(ain[pin], &value);
		if(rc == RP_OK)
		{
			//scale read value from 0-156 to 0-255
			scaled_value = value*RP_AREAD_MAX_VALUE/AREAD_MAX_VALUE;
			return scaled_value;
		}
		printf("%s\n", rp_GetError(rc));
	}
	else
		printf("Pin %d is not analog input\n",pin);
	return -1;
}

int analogReadRaw (int pin)
{
	if(pin<AIN)
	{
		int value;
		int rc = rp_ApinGetValueRaw(ain[pin], &value);
		if(rc == RP_OK)
			return value;
		printf("%s\n", rp_GetError(rc));
	}
	else
		printf("Pin %d is not analog input\n",pin);
	return -1;
}

float analogReadVoltage (int pin)
{
	if(pin<AIN)
	{
		float value;
		int rc = rp_ApinGetValue(ain[pin], &value);
		if(rc == RP_OK)
			return value;
		printf("%s\n", rp_GetError(rc));
	}
	else
		printf("Pin %d is not analog input\n",pin);
	return -1;
}

/**************************************************************************************************
 * delays
 *************************************************************************************************/

void delay (unsigned int milliseconds)
{
	usleep (milliseconds*1000);
}

void delayMicroseconds (unsigned int microseconds)
{
	usleep (microseconds);
}

unsigned int millis()
{
   return micros() / 1000;
}


unsigned int micros()
{

  struct timespec t;
  t.tv_sec = t.tv_nsec = 0;

  clock_gettime(CLOCK_REALTIME, &t);
  return (unsigned long)(t.tv_sec)*1000000L + t.tv_nsec / 1000L ;

}

unsigned long pulseIn(uint8_t pin, uint8_t state)
{
	printf ("pulseIn is not implemented for Red Pitaya");
  	return 0;
}

/**
 * Shifts in a byte of data one bit at a time. Starts from either the most (i.e. the leftmost) or 
 * least (rightmost) significant bit. For each bit, the clock pin is pulled high, the next bit is
 * read from the data line, and then the clock pin is taken low.
 * If you're interfacing with a device that's clocked by rising edges, you'll need to make sure
 * that the clock pin is low before the first call to shiftIn(), e.g. with a call to 
 * digitalWrite(clockPin, LOW).
 */
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
  uint8_t value;
  uint8_t i;

  value = 0;
  for (i = 0; i < 8; i++) {
    digitalWrite(clockPin, HIGH);

    if (bitOrder == LSBFIRST) {
      value |= digitalRead(dataPin) << i;
    } else {
      value |= digitalRead(dataPin) << (7 - i);
    }

    digitalWrite(clockPin, LOW);
  }

  return value;
}

/**
 * Shifts out a byte of data one bit at a time. Starts from either the most (i.e. the leftmost)
 * or least (rightmost) significant bit. Each bit is written in turn to a data pin, after which a
 * clock pin is pulsed (taken high, then low) to indicate that the bit is available.
 */
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
  uint8_t i;

  for (i = 0; i < 8; i++) {
    if (bitOrder == LSBFIRST) {
      digitalWrite(dataPin, !!(val & (1 << i)));
    } else {
      digitalWrite(dataPin, !!(val & (1 << (7 - i))));
    }
        
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }
}

/**************************************************************************************************
 * 6.SPI
 *************************************************************************************************/
int spi_getadapter(uint32_t spi_bus_address) {}
int spi_openadapter(uint8_t spi_bus) {}
int spi_setmode(int spi_bus, unsigned short mode) {}
int spi_set_frequency(int spi_bus, int freq) {}
uint8_t spi_writebyte(int spi_bus, uint8_t byte) {}
unsigned char * spi_writebytes(int spi_bus, uint8_t *bytes, uint8_t length) {}
int spi_lsb_mode(int spi_bus, unsigned char lsb) {}
int spi_bit_per_word(int spi_bus, unsigned int bits) {}
int spi_closeadapter (int spi_bus) {}

/**************************************************************************************************
 * 7.I2C
 *************************************************************************************************/
// functions are i2c.c



/**************************************************************************************************
 * 8.Serial
 *************************************************************************************************/
int serial_openadapter(char *serial_bus) {}
int serial_set_speed(int serial_id, int baud) {}
int serial_bytes_available(int serial_id) {}
int serial_closeadapter(int serial_id) {}
int serial_writebyte(int serial_id, uint8_t byte) {}
int serial_writebytes(int serial_id, uint8_t *bytes, uint8_t length) {}
uint8_t serial_readbyte(int serial_id) {}
int serial_readbytes(int serial_id, uint8_t *buf, int length) {}
int serial_flush(int serial_id) {}


#ifdef __cplusplus
}
#endif

 #endif
