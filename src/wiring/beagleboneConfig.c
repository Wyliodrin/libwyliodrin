#ifdef BEAGLEBONE

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "beagleboneConfig.h"

/******************************************************************************
 * CONTENT
 *
 * 1.GPIO
 *****************************************************************************/



/******************************************************************************
 * 1.GPIO
 *****************************************************************************/

/**
 * Test function
 */
void beagleTest() {
  printf("Hello from beagleboneConfig.c! Magic number: 7\n");
}

/**
 * Returns the gpio pin number by name or -1 in case of failure
 */
int getGpioByName(const char *name) {
  int i;
  pin_t *aux;

  aux = pinTable;
  for (i = 0; i < sizeof(pinTable)/sizeof(pinTable[0]); i++) {
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
  for (i = 0; i < sizeof(pinTable)/sizeof(pinTable[0]); i++) {
    if(strcmp(aux->key, key) == 0) {
      return aux->gpio;
    }

    aux++;
  }

  debug("There is no pin with the key %s", key);
  return -1;
}

/**
 * Exports pin
 */
void gpioExport(uint gpio) {
  int fd, len;
  char buf[MAX_BUF];

  fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
  if (fd < 0) {
    perror("gpioExport");
    return;
  }

  len = snprintf(buf, sizeof(buf), "%d", gpio);
  write(fd, buf, len);
  close(fd);
}

/**
 * Unexports pin
 */
void gpioUnexport(uint gpio) {
  int fd, len;
  char buf[MAX_BUF];

  fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
  if (fd < 0) {
    perror("gpioUnexport");
    return;
  }

  len = snprintf(buf, sizeof(buf), "%d", gpio);
  write(fd, buf, len);
  close(fd);
}

/**
 * Sets direction of pin
 */
void gpioSetDir(uint gpio, PIN_DIRECTION dir) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction", gpio);

  fd = open(buf, O_WRONLY);
  if (fd < 0) {
    perror("gpioSetDir");
    return;
  }

  if (dir == OUTPUT) {
    write(fd, "out", 4);
  } else {
    write(fd, "in", 3);
  }

  close(fd);
}

// TODO
int  gpioGetDir    (uint gpio) {}
int  gpioSetValue  (uint gpio, PIN_VALUE value) {}
int  gpioGetValue  (uint gpio) {}
int  gpioSetEdge   (uint gpio, char *edge) {}
int  gpioFdOpen    (uint gpio) {}
int  gpioFdClose   (int fd) {}

int  gpioOmapMuxSetup (const char *omap_pin0_name, const char *mode) {}



#ifdef __cplusplus
}
#endif

#endif // BEAGLEBONE
