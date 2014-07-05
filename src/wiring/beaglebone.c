#ifdef BEAGLEBONE

#include "beaglebone.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Digital I/O
 *****************************************************************************/

/**
 * DESCRIPTION:
 * Configures the specified pin to behave either as an input or an output.
 * See pinTable in beaglebone.h for details on the functionality of the pins.
 *
 * PARAMETERS:
 * pin  - the number of the pin whose pin you wish to set 
 *        (gpio in pins_t struct from beaglebone.h)
 * mode - INPUT or OUTPUT
 */

void pinMode (int pin, int mode) {
  // TODO
}

void digitalWrite (int pin, int value) {
  // TODO
}

int digitalRead (int pin) {
  // TODO

  return 0;
}

/******************************************************************************
 * Analog I/O
 *****************************************************************************/

void analogWrite (int pin, int value) {
  // TODO
}

int analogRead (int pin) {
  // TODO
}

#ifdef __cplusplus
}
#endif

#endif // BEAGLEBONE
