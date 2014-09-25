
#ifndef WIRING__H
#define WIRING__H

#define	LSBFIRST	0
#define	MSBFIRST	1
#define MAX_I2C_BUSES 10
#define MAX_SPI_BUSES 10
#define MAX_SERIAL_BUSES	10

#define RUN_RASPBERRYPI 0
#define RUN_GALILEO 1
#define RUN_EDISON 2
#define RUN_BEAGLEBONE 3

#ifdef RASPBERRYPI
#define BOARD RUN_RASPBERRYPI
#endif

#ifdef ARDUINOGALILEO
#define BOARD RUN_GALILEO
#endif

#ifdef EDISON
#define BOARD RUN_EDISON
#endif

#ifdef BEAGLEBONE
#define BOARD RUN_BEAGLEBONE
#endif


typedef unsigned char uint8_t;

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

// Raspberry Pi
// use wiring library
#ifdef RASPBERRYPI
#include <wiringPi.h>
#include "raspberrypi.h"
#define MAX_I2C_PINS	10
#endif

// Arduino Galileo
// use the mraa library
#if defined(ARDUINOGALILEO) || defined(EDISON)

#include <mraa/gpio.h>
#include <mraa/aio.h>
#include <mraa/pwm.h>
#include <mraa/i2c.h>
#include <mraa/spi.h>

#define MAX_GPIO_PINS 100
#define MAX_AIO_PINS 40
#define MAX_PWM_PINS 50

#define A0	14
#define A1	15
#define A2	16
#define A3	17
#define A4	18
#define A5	19
#define A6	20

#endif

#define LSBFIRST  0
#define MSBFIRST  1

#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1

#include "binary.h"
#include "BitsAndBytes.h"

typedef uint8_t byte;
typedef unsigned short uint16_t;

#ifdef __cplusplus
extern "C" {
#endif

int wiringSetup ();

void pinReset (int pin);

// WIRING
int getPinByName(const char* name);
int getPinByKey(const char* key);
void pinMode (int pin, int mode);
void digitalWrite (int pin, int value);
int digitalRead (int pin);
void analogWrite (int pin, int value);
int analogRead (int pin);

//#ifndef RASPBERRYPI 
void delay (unsigned int milliseconds);
void delayMicroseconds (unsigned int microseconds);
unsigned int millis (void);
unsigned int micros (void);
//#endif

// Shift

// unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
// void tone(uint8_t _pin, unsigned int frequency, unsigned long duration);
// void noTone(uint8_t _pin);

// Math
long randomMinMax( long, long ) ;

long map( long, long, long, long, long ) ;

uint16_t makeWord( uint8_t h, uint8_t l ) ;

#define word(...) makeWord(__VA_ARGS__)

// SPI

// TODO
int spi_getadapter(uint32_t spi_bus_address);
int spi_openadapter(uint8_t spi_bus);
int spi_setmode(int spiId, unsigned short mode);
int spi_set_frequency(int spiId, int freq);
uint8_t spi_writebyte(int spiId, uint8_t byte);
unsigned char * spi_writebytes(int spiId, uint8_t *bytes, uint8_t length);
int spi_lsb_mode(int spiId, unsigned char lsb);
int spi_bit_per_word(int spiId, unsigned int bits);
int spi_closeadapter (int spiId);

// I2C

// int i2c_getadapter(uint32_t i2c_bus_address);
int i2c_openadapter(uint8_t i2c_bus);
int i2c_setslave(int i2cId, uint8_t addr);
int i2c_writebyte(int i2cId, uint8_t byte);
int i2c_writebytes(int i2cId, uint8_t *bytes, uint8_t length);
int i2c_readbyte(int i2cId);
int i2c_closeadapter (int i2cId);
int i2c_readbytes(int i2cId, uint8_t *buf, int length);
int i2c_readwrite(int i2cId);
// int i2c_add_to_buf(uint8_t addr, uint8_t rw, uint8_t *value, int length);

// Serial

int serial_openadapter(char *serial_bus);
int serial_set_speed(int serial_id, int baud);
int serial_bytes_available(int serial_id);
int serial_closeadapter(int serial_id);
int serial_writebyte(int serial_id, uint8_t byte);
int serial_writebytes(int serial_id, uint8_t *bytes, uint8_t length);
uint8_t serial_readbyte(int serial_id);
int serial_readbytes(int serial_id, uint8_t *buf, int length);
int serial_flush(int serial_id);
/* TODO if needed
	availalble
	find
	findUntil
	parseFloat
	parseInt
	peek
	readBytesUntil
	setTimeout-nu se prea poate din wiringpi
	serialEvent
*/

#ifdef __cplusplus
}
#endif

#endif

