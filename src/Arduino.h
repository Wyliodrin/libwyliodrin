/**************************************************************************************************
 * Arduino setup() and loop() functions
 *************************************************************************************************/

#ifndef ARDUINO_H
#define ARDUINO_H

#include <Wyliodrin.h>
#include <stdlib.h>
#include <string.h>

#define PROGMEM

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

#ifdef __cplusplus
extern "C" {
#endif

void setup();
void loop();

#ifdef __cplusplus
}
#endif

int main ()
{
  wyliodrinSetup();
  setup();
  for(;;)
  {
    loop();
  }
}

#endif // ARDUINO_H
