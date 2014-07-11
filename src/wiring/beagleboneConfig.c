#ifdef BEAGLEBONE

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "beagleboneConfig.h"

/**************************************************************************************************
 * This file contains the definitions of the GPIO configuration functions for the BeagleBone Black
 *
 * CONTENT
 * 1.GPIO
 *************************************************************************************************/



/**************************************************************************************************
 * 1.GPIO
 *************************************************************************************************/

/**
 * Function used for testing the correctness of the install
 */
void beagleTest() {
  printf("Hello from beagleboneConfig.c! Magic number: 7\n");
}

/**
 * Returns the gpio pin number by name or -1 in case of failure
 */
uint getGpioByName(const char *name) {
  int i;
  pin_t *aux;

  aux = pinTable;
  for(i = 0; i < sizeof(pinTable)/sizeof(pinTable[0]); i++) {
    if(strcmp(aux->name, name) == 0) {
      return aux->gpio;
    }

    aux++;
  }

  debug("There is no pin named %s", name);
  return 0;
}

/**
 * Returns the gpio pin number by key or -1 in case of failure
 */
uint getGpioByKey(const char *key) {
  int i;
  pin_t *aux;

  aux = pinTable;
  for(i = 0; i < sizeof(pinTable)/sizeof(pinTable[0]); i++) {
    if(strcmp(aux->key, key) == 0) {
      return aux->gpio;
    }

    aux++;
  }

  debug("There is no pin with the key %s", key);
  return 0;
}

/**
 * Exports pin
 */
void gpioExport(uint gpio) {
  int fd, len;
  char buf[MAX_BUF];

  fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
  if(fd < 0) {
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
  if(fd < 0) {
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
void gpioSetDir(uint gpio, pin_direction_t dir) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction", gpio);

  fd = open(buf, O_WRONLY);
  if(fd < 0) {
    perror("gpioSetDir");
    return;
  }

  if(dir == OUTPUT) {
    write(fd, "out", 4);
  } else {
    write(fd, "in", 3);
  }

  close(fd);
}

/**
 * Returns direction of pin
 *
 * 0 INPUT
 * 1 OUTPUT
 */
int gpioGetDir(uint gpio) {
  int fd;
  char buf[MAX_BUF];
  char ch;

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction", gpio);

  fd = open(buf, O_RDONLY);
  if(fd < 0) {
    perror("gpioGetDir");
    return -1;
  }

  memset(buf, 0, MAX_BUF);
  read(fd, buf, MAX_BUF);
  close(fd);

  if(strncmp(buf, "in", 2) == 0) {
    return 0; // INPUT
  } else if(strncmp(buf, "out", 3) == 0) {
    return 1; // OUTPUT
  } else {
    debug("Unknown direction %s", buf);
    return -1;
  }
}

/**
 * Sets value of pin
 */
void gpioSetValue(uint gpio, pin_value_t value) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

  fd = open(buf, O_WRONLY);
  if(fd < 0) {
    perror("gpioSetValue");
    return;
  }

  if (value == LOW)
    write(fd, "0", 2);
  else
    write(fd, "1", 2);

  close(fd);
}

/**
 * Returns value of pin
 * 
 * 0 LOW
 * 1 HIGH
 */
int gpioGetValue(uint gpio) {
  int fd;
  char buf[MAX_BUF];
  char ch;

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

  fd = open(buf, O_RDONLY);
  if(fd < 0) {
    perror("gpioGetValue");
    return;
  }

  read(fd, &ch, 1);
  close(fd);

  if(ch == '0') {
    return 0; // LOW
  } else if(ch == '1') {
    return 1; // HIGH
  } else {
    debug("Unknown value %c", ch);
  }
}

/**
 * Sets edge of pin
 */
void gpioSetEdge(uint gpio, edge_t edge) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

  fd = open(buf, O_WRONLY);
  if (fd < 0) {
    perror("gpioSetEdge");
    return;
  }

  if(edge == NONE) {
    write(fd, "none", 5);
  } else if(edge == RISING) {
    write(fd, "rising", 7);
  } else if(edge == FALLING) {
    write(fd, "falling", 8);
  } else if(edge == BOTH) {
    write(fd, "both", 5);
  } else {
    debug("Edge can be NONE, RISING, FALLING or BOTH");
  }

  close(fd);
}

/**
 * Returns edge of pin
 */
edge_t gpioGetEdge(uint gpio) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

  fd = open(buf, O_RDONLY);
  if (fd < 0) {
    perror("gpioGetEdge");
    return;
  }

  memset(buf, 0, MAX_BUF);
  read(fd, buf, MAX_BUF);
  close(fd);

  if(strncmp(buf, "none", 4) == 0) {
    return NONE;
  } else if(strncmp(buf, "rising", 6) == 0) {
    return RISING;
  } else if(strncmp(buf, "falling", 7) == 0) {
    return FALLING;
  } else if(strncmp(buf, "both", 4) == 0) {
    return BOTH;
  } else {
    debug("Unknown edge %s", buf);
    return NONE;
  }
}



#ifdef __cplusplus
}
#endif

#endif // BEAGLEBONE
