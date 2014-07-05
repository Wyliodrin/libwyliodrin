#ifdef BEAGLEBONE

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * CONTENT
 *
 * 1.Digital I/O
 * 2.Analog I/O
 *****************************************************************************/


/******************************************************************************
 * 1.Digital I/O
 *****************************************************************************/

/**
 * Configures the specified pin to behave either as an input or an output.
 *
 * See 2.Pin Configuration and 3.Helper Functions in beaglebone.h for details
 * on the functionality of the pins.
 *
 * PARAMETERS:
 * pin  - the number of the pin whose pin you wish to set 
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
 * 2.Analog I/O
 *****************************************************************************/

void analogWrite (int pin, int value) {
  // TODO
}

int analogRead (int pin) {
  // TODO

  return 0;
}

#ifdef __cplusplus
}
#endif

#endif // BEAGLEBONE
