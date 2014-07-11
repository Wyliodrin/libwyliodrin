#ifdef BEAGLEBONE

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "beagleboneConfig.h"
#include "wiring.h"

/**************************************************************************************************
 * This file contains the definitions of all wiring functions for the BeagleBone Black
 *
 * CONTENT
 * 1.General
 * 2.Digital I/O
 * 3.Analog I/O
 * 4.Advanced I/O
 * 5.Time
 * 6.SPI
 * 7.I2C
 *************************************************************************************************/



/**************************************************************************************************
 * 1.General
 *************************************************************************************************/

/**
 * TODO
 */
int wiringSetup() {
  // TODO

  return 0;
}

/**
 * Resets pin
 */
void pinReset(int pin) {
  gpioUnexport(pin);
}



/**************************************************************************************************
 * 2.Digital I/O
 *************************************************************************************************/

/**
 * Configures the specified pin to behave either as an input or an output.
 *
 * PARAMETERS:
 * pin  - the number of the pin whose pin you wish to set 
 * mode - 0 for INPUT or 1 for OUTPUT
 */
void pinMode(int pin, int mode) {
  pin_direction_t dir;

  gpioExport(pin);

  if(mode == 0) {
    dir = INPUT;
  } else if(mode == 1) {
    dir = OUTPUT;
  } else {
    debug("Wrong mode %d. Mode can be either INPUT or OUTPUT", mode);
    return;
  }

  gpioSetDir(pin, dir);
}

/**
 * Writes a HIGH or a LOW value to a digital pin.
 *
 * PARAMETERS:
 * pin   - the pin number
 * value - 0 for LOW or 1 for HIGH
 */
void digitalWrite(int pin, int value) {
  pin_value_t val;

  if(!gpioIsExported(pin)) {
    debug("pin %d is not exported. You should call pinMode before digitalWrite", pin);
    return;
  }

  if(value == 0) {
    val = LOW;
  } else if(value == 1) {
    val = HIGH;
  } else {
    debug("Wrong value %d. Value can be either LOW or HIGH", value);
    return;
  }

  gpioSetValue(pin, val);
}

/**
 * Reads the value from a specified digital pin, either HIGH or LOW.
 *
 * PARAMETERS:
 * pin: the number of the digital pin you want to read (int)
 *
 * RETURNS: 
 * 1 for HIGH or 0 for LOW
 */
int digitalRead(int pin) {
  if(!gpioIsExported(pin)) {
    debug("pin %d is not exported. You should call pinMode before digitalRead", pin);
    return -1;
  }

  return gpioGetValue(pin);
}



/**************************************************************************************************
 * 3.Analog I/O
 *************************************************************************************************/

/**
 * TODO
 */
void analogWrite(int pin, int value) {
  // TODO
}

/**
 * TODO
 */
int analogRead(int pin) {
  // TODO

  return 0;
}



/**************************************************************************************************
 * 4.Advanced I/O
 *************************************************************************************************/

/**
 * TODO
 */
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
  // TODO

  return 0;
}

/**
 * TODO
 */
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
  // TODO
}



/**************************************************************************************************
 * 5.Time
 *************************************************************************************************/

/**
 * Pauses the program for the amount of time (in miliseconds) specified as parameter. 
 * There are 1000 milliseconds in a second.
 *
 * PARAMETERS:
 * milliseconds: the number of milliseconds to pause
 */
void delay(unsigned int milliseconds) {
  // TODO
}

/**
 * Pauses the program for the amount of time (in microseconds) specified as parameter. 
 * There are a thousand microseconds in a millisecond, and a million microseconds in a second.
 *
 * PARAMETERS:
 * microseconds: the number of microseconds to pause
 */
void delayMicroseconds(unsigned int microseconds) {
  // TODO
}

/**
 * Returns the number of milliseconds since the Beaglebone board began running the current program.
 *
 * This number will overflow (go back to zero), after approximately 50 days.
 */
unsigned int millis() {
  // TODO

  return 0;
}

/**
 * Returns the number of microseconds since the Beaglebone board began running the current program.
 *
 * This number will overflow (go back to zero), after approximately 70 minutes.
 */
unsigned int micros() {
  // TODO

  return  0;
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
// int i2c_getadapter(uint32_t i2c_bus_address) {}
int i2c_openadapter(uint8_t i2c_bus) {}
int i2c_setslave(int i2c_bus, uint8_t addr) {}
int i2c_writebyte(int i2c_bus, uint8_t byte) {}
int i2c_writebytes(int i2c_bus, uint8_t *bytes, uint8_t length) {}
int i2c_readbyte(int i2c_bus) {}
int i2c_closeadapter (int i2c_bus) {}
int i2c_readbytes(int i2c_bus, uint8_t *buf, int length) {}
int i2c_readwrite(int i2c_bus) {}
// int i2c_add_to_buf(uint8_t addr, uint8_t rw, uint8_t *value, int length) {}


#ifdef __cplusplus
}
#endif

#endif // BEAGLEBONE
