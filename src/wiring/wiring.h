#ifndef _WIRING_H
#define _WIRING_H



/*** DEFINES *************************************************************************************/

#define LSBFIRST  0
#define MSBFIRST  1

#define MAX_I2C_BUSES    10
#define MAX_SPI_BUSES    10
#define MAX_SERIAL_BUSES 10

#define RUN_RASPBERRYPI 0
#define RUN_GALILEO     1
#define RUN_EDISON      2
#define RUN_BEAGLEBONE  3
#define RUN_SERVER      5

#ifdef RASPBERRYPI
  #define BOARD RUN_RASPBERRYPI
#endif /* RASPBERRYPI */

#ifdef ARDUINOGALILEO
  #define BOARD RUN_GALILEO
#endif /* ARDUINOGALILEO */

#ifdef EDISON
  #define BOARD RUN_EDISON
#endif /* EDISON */

#ifdef BEAGLEBONE
  #define BOARD RUN_BEAGLEBONE
#endif /* BEAGLEBONE */

#ifdef SERVER
  #define BOARD RUN_SERVER
#endif /* SERVER */

#ifdef REDPITAYA
  #define BOARD RUN_REDPITAYA
#endif /* REDPITAYA */

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

#define LSBFIRST  0
#define MSBFIRST  1

#define HIGH   1
#define LOW    0
#define INPUT  0
#define OUTPUT 1

/*************************************************************************************************/



/*** INCLUDES ************************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "binary.h"
#include "BitsAndBytes.h"

#ifdef RASPBERRYPI
  #include <wiringPi.h>
  #include "raspberrypi.h"
  #define MAX_I2C_PINS  10
#endif /* RASPBERRYPI */

#ifdef UDOONEO
  #define MAX_I2C_PINS  10
#endif /* UDOONEO */

#if defined(DEVICEINTEL)//defined(ARDUINOGALILEO) || defined(EDISON)
  #include <mraa/gpio.h>
  #include <mraa/aio.h>
  #include <mraa/pwm.h>
  #include <mraa/i2c.h>
  #include <mraa/spi.h>

  #define MAX_GPIO_PINS 100
  #define MAX_AIO_PINS 40
  #define MAX_PWM_PINS 50

  #define A0  14
  #define A1  15
  #define A2  16
  #define A3  17
  #define A4  18
  #define A5  19
  #define A6  20
#endif /* defined(ARDUINOGALILEO) || defined(EDISON) */

#ifdef REDPITAYA
  #define A0  0
  #define A1  1
  #define A2  2
  #define A3  3
#endif /* REDPITAYA */

/*************************************************************************************************/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



/*** API *****************************************************************************************/

#ifdef REDPITAYA
  void  analogWriteVoltage (int pin, float value);
  float analogReadVoltage  (int pin);
#endif /* REDPITAYA */

int wiringSetup();

void pinReset(int pin);

/* I/O */
void pinMode        (int pin, int mode);
void digitalWrite   (int pin, int value);
int  digitalRead    (int pin);
void analogWrite    (int pin, int value);
int  analogRead     (int pin);
int  analogReadRaw  (int pin);
void analogWriteRaw (int pin, int value);

/* Advanced I/O */
void          shiftOut (uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t       shiftIn  (uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
unsigned long pulseIn  (uint8_t pin, uint8_t state);

/* Time */
void delay             (unsigned int milliseconds);
void delayMicroseconds (unsigned int microseconds);
unsigned int millis    (void);
unsigned int micros    (void);

long randomMinMax(long, long);

long map(long, long, long, long, long);

uint16_t makeWord(uint8_t h, uint8_t l);

#define word(...) makeWord(__VA_ARGS__)

int             spi_getadapter    (uint32_t spi_bus_address);
int             spi_openadapter   (uint8_t spi_bus);
int             spi_setmode       (int spiId, unsigned short mode);
int             spi_set_frequency (int spiId, int freq);
uint8_t         spi_writebyte     (int spiId, uint8_t byte);
unsigned char * spi_writebytes    (int spiId, uint8_t *bytes, uint8_t length);
int             spi_lsb_mode      (int spiId, unsigned char lsb);
int             spi_bit_per_word  (int spiId, unsigned int bits);
int             spi_closeadapter  (int spiId);

int i2c_openadapter  (uint8_t i2c_bus);
int i2c_setslave     (int i2cId, uint8_t addr);
int i2c_writebyte    (int i2cId, uint8_t byte);
int i2c_writebytes   (int i2cId, uint8_t *bytes, uint8_t length);
int i2c_readbyte     (int i2cId);
int i2c_closeadapter (int i2cId);
int i2c_readbytes    (int i2cId, uint8_t *buf, int length);
int i2c_readwrite    (int i2cId);


int     serial_openadapter     (char *serial_bus);
int     serial_set_speed       (int serial_id, int baud);
int     serial_bytes_available (int serial_id);
int     serial_closeadapter    (int serial_id);
int     serial_writebyte       (int serial_id, uint8_t byte);
int     serial_writebytes      (int serial_id, uint8_t *bytes, uint8_t length);
uint8_t serial_readbyte        (int serial_id);
int     serial_readbytes       (int serial_id, uint8_t *buf, int length);
int     serial_flush           (int serial_id);

/*************************************************************************************************/



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _WIRING_H */
