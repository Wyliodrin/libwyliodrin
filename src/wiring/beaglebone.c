#ifdef BEAGLEBONE

#include "beaglebone.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Digital I/O
 *****************************************************************************/

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
