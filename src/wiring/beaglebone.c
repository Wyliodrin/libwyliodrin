/**************************************************************************************************
 * Author: Razvan Madalin MATEI <matei.rm94@gmail.com>
 * Date last modified: July 2014
 * 
 * This file contains the definitions of all wiring functions for the BeagleBone Black.
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

#ifdef BEAGLEBONE

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "beagleboneConfig.h"
#include "wiring.h"



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
  if(ledIsValid(pin)) {
    ledReset(pin);
    return;
  }

  gpioUnexport(pin);
}

/**
 * Returns PIN by name
 */
int getPinByName(const char* name) {
  return getGpioByName(name);
}

/**
 * Returns PIN by key
 */
int getPinByKey(const char* key) {
  return getGpioByKey(key);
}



/**************************************************************************************************
 * 2.Digital I/O
 *************************************************************************************************/

/**
 * Configures the specified pin to behave either as an input or an output.
 *
 * PARAMETERS:
 * pin  - the number of the pin whose pin you wish to set 
 * mode - INPUT or OUTPUT
 */
void pinMode(int pin, int mode) {
  // Test valid pin
  if(!gpioIsValid(pin)) {
    debug("Invalid pin %d", pin);
    return;
  }

  // Test valid mode
  if(mode != INPUT && mode != OUTPUT) {
    debug("Mode can be either INPUT or OUTPUT");
    return;
  }

  // Handle case where pin is allocated as a gpio-led
  if(isLed(pin)) {
    if(mode != OUTPUT) {
      debug("pinMode only supports mode OUTPUT for LEDs");
    } else {
      ledSetTrigger(pin, NONE);
    }
    return;
  }

  gpioExport(pin);
  gpioSetDir(pin, mode);
}

/**
 * Writes a HIGH or a LOW value to a digital pin.
 *
 * PARAMETERS:
 * pin   - the pin number
 * value - LOW or HIGH
 */
void digitalWrite(int pin, int value) {
  // Test valid pin
  if(!gpioIsValid(pin)) {
    debug("Invalid pin %d. See pinTable in beagleboneConfig.c.", pin);
    return;
  }

  // Test valid value
  if(value != LOW && value != HIGH) {
    debug("Value can be either LOW or HIGH");
    return;
  }

  // Handle case where pin is allocated as a gpio-led
  if(isLed(pin)) {
    ledSetValue(pin, value);
    return;
  }

  // Test if pin is exported
  if(!gpioIsExported(pin)) {
    debug("Pin %d is not exported. pinMode should be called before digitalWrite.", pin);
    return;
  }

  gpioSetValue(pin, value);
}

/**
 * Reads the value from a specified digital pin, either HIGH or LOW.
 *
 * PARAMETERS:
 * pin: the number of the digital pin you want to read
 *
 * RETURNS: 
 * LOW or HIGH
 */
int digitalRead(int pin) {
  // Test valid pin
  if(!gpioIsValid(pin)) {
    debug("Invalid pin %d. See pinTable in beagleboneConfig.c.", pin);
    return;
  }

  // Test if pin is exported
  if(!gpioIsExported(pin)) {
    debug("pin %d is not exported. You should call pinMode before digitalRead", pin);
    return -1;
  }

  // Handle case where pin is allocated as a gpio-led
  if(isLed(pin)) {
    return ledGetValue(pin);
  }

  return gpioGetValue(pin);
}



/**************************************************************************************************
 * 3.Analog I/O
 *************************************************************************************************/

/**
 * Writes an analog value (PWM wave) to a pin. 
 *
 * Can be used to light a LED at varying brightnesses or drive a motor at various speeds.
 *
 * After a call to analogWrite(), the pin will generate a steady square wave of the specified duty
 * cycle until the next call to analogWrite() (or a call to digitalRead() or digitalWrite() on the
 * same pin).
 */
void analogWrite(int pin, int value) {
  if(!(0 <= value && value <= 255)) {
    debug("Value should be in [0, 255] interval");
    return;
  }

  if(!pwmIsValid(pin)) {
    debug("Pin %d is not a valid PWM pin", pin);
    return;
  }

  char *key;
  ulong duty;
  ulong period;

  key = strdup(getKeyByGpio(pin));
  pwmEnable(key);

  period = pwmGetPeriod(key);
  duty   = (1.0 - value / 255.0) * period;

  pwmSetDuty(key, duty);
}

/**
 * Reads the voltage at an analog input pin.
 * The Beaglebone Black has 7 analog input pins.
 * Maximum input voltage is 1.8V.
 * 
 * PARAMETERS:
 * pin - analog pin
 *
 * RETURN:
 * value between 0 and 100 (coresponding to 1.8V)
 */
int analogRead(int pin) {
  if(!(200 <= pin && pin <= 206)) {
    debug("Value of pin should be in [200, 206] interval");
    return -1;
  }

  ainInit();
  return ainGetValue(pin) / 18;
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
  usleep(milliseconds * 1000);
}

/**
 * Pauses the program for the amount of time (in microseconds) specified as parameter. 
 * There are a thousand microseconds in a millisecond, and a million microseconds in a second.
 *
 * PARAMETERS:
 * microseconds: the number of microseconds to pause
 */
void delayMicroseconds(unsigned int microseconds) {
  usleep(microseconds);
}

/**
 * Returns the number of milliseconds since the Beaglebone board began running the current program.
 *
 * This number will overflow (go back to zero), after approximately 50 days.
 */
unsigned int millis() {
  return micros() / 1000;
}

/**
 * Returns the number of microseconds since the Beaglebone board began running the current program.
 *
 * This number will overflow (go back to zero), after approximately 70 minutes.
 */
unsigned int micros() {
  struct timespec t;

  t.tv_sec  = 0;
  t.tv_nsec = 0;

  clock_gettime(CLOCK_REALTIME, &t);

  return (unsigned long)(t.tv_sec) * 1000000L + t.tv_nsec / 1000L ;
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
