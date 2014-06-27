
#ifndef WIRING__H
#define WIRING__H

typedef unsigned char uint8_t;

// Raspberry Pi
// use wiring library
#ifdef RASPBERRYPI
#include <wiringPi.h>
#endif

// Arduino Galileo
// use the mraa library
#ifdef ARDUINOGALILEO

#include <mraa/gpio.h>
#include <mraa/aio.h>
#include <mraa/pwm.h>
#include <mraa/i2c.h>
#include <mraa/spi.h>
#include <unistd.h>

#define MAX_GPIO_PINS 100
#define MAX_AIO_PINS 40
#define MAX_PWM_PINS 50
#define MAX_I2C_PINS 10
#define MAX_SPI_PINS 10

#define	LSBFIRST	0
#define	MSBFIRST	1

#define A0	14
#define A1	15
#define A2	16
#define A3	17
#define A4	18
#define A5	19
#define A6	20

#endif

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

void pinMode (int pin, int mode);
void digitalWrite (int pin, int value);
int digitalRead (int pin);
void analogWrite (int pin, int value);
int analogRead (int pin);

#ifndef RASPBERRYPI 
void delay (int milliseconds);
void delayMicroseconds (int microseconds);
unsigned long millis ();
unsigned long micros ();
#endif

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

// I2C

// int i2c_getadapter(uint32_t i2c_bus_address);
int i2c_openadapter(uint8_t i2c_bus);
int i2c_setslave(int i2c_bus, uint8_t addr);
int i2c_writebyte(int i2c_bus, uint8_t byte);
int i2c_writebytes(int i2c_bus, uint8_t *bytes, uint8_t length);
int i2c_readbyte(int i2c_bus);
int i2c_readbytes(int i2c_bus, uint8_t *buf, int length);
int i2c_readwrite(int i2c_bus);
// int i2c_add_to_buf(uint8_t addr, uint8_t rw, uint8_t *value, int length);

#ifdef __cplusplus
}
#endif

#endif

