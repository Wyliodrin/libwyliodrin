#ifdef BEAGLEBONE

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "beagleboneConfig.h"

/******************************************************************************
 * CONTENT
 *
 * 1.Helper functions
 *****************************************************************************/



/******************************************************************************
 * 1.Helper functions
 *****************************************************************************/

/**
 * Test function
 */
void beagleTest() {
  printf("Hello from beagleboneConfig.c!\n");
}

/**
 * Returns the gpio pin number by name or -1 in case of failure
 */
int getGpioByName(const char *name) {
  int i;
  pin_t *aux;

  aux = pinTable;
  for (i = 0; i < sizeof(pinTable)/sizeof(pin_t); i++) {
    if(strcmp(aux->name, name) == 0) {
      return aux->gpio;
    }

    aux++;
  }

  debug("There is no pin named %s", name);
  return -1;
}

/**
 * Returns the gpio pin number by key or -1 in case of failure
 */
int getGpioByKey(const char *key) {
  int i;
  pin_t *aux;

  aux = pinTable;
  for (i = 0; i < sizeof(pinTable)/sizeof(pin_t); i++) {
    if(strcmp(aux->key, key) == 0) {
      return aux->gpio;
    }

    aux++;
  }

  debug("There is no pin with the key %s", key);
  return -1;
}

#ifdef __cplusplus
}
#endif

#endif // BEAGLEBONE
