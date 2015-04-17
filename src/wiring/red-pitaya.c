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

#include <rp.h>

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



void pinMode(int pin, int mode)
{
	int rc = rp_DpinSetDirection(pin, mode);
	if(rc != RP_OK)
		printf("%s\n", rp_GetError(rc));
}
}

void digitalWrite (int pin, int value)
{
	int rc = rp_DpinSetState(pin, value);
	if(rc != RP_OK)
		printf("%s\n", rp_GetError(rc));
}

int digitalRead (int pin)
{
	int value;
	int rc = rp_DpinGetState(pin, &value);
	if(rc == RP_OK)
		return value;
	printf("%s\n", rp_GetError(rc));
	return -1;
}

void analogWrite (int pin, int value)
{
	int rc = rp_ApinSetValueRaw(pin, value);
	if(rc != RP_OK)
		printf("%s\n", rp_GetError(rc));
}

int analogRead (int pin)
{
	int value;
	int rc = rp_ApinGetValueRaw(pin, &value);
	if(rc == RP_OK)
		return value;
	printf("%s\n", rp_GetError(rc));
	return -1;
}

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
	printf ("pulseIn is not implemented for BeagleBone Black");
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