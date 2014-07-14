#ifndef BEAGLEBONE_CONFIG_H
#define BEAGLEBONE_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 * This file contains the declarations of the GPIO configuration functions for the BeagleBone Black
 *
 * CONTENT:
 * 1.Constants
 * 2.GPIO
 *************************************************************************************************/



/**************************************************************************************************
 * 1.Constants
 *************************************************************************************************/

#define MAX_BUF 64

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define SYSFS_LEDS_DIR "/sys/class/leds"

#define INPUT  0
#define OUTPUT 1

#define LOW  0
#define HIGH 1

#define NONE    0
#define RISING  1
#define FALLING 2
#define BOTH    3

#define isLed(gpio) (53 <= (gpio) && (gpio) <= 56)

// If they'll like this macro, I'll move it in a more generic file
#define DEBUG 1
#define debug(...)                                                                 \
  do {                                                                             \
    if (DEBUG) {                                                                   \
      fprintf(stderr, "Debug alert in file %s at line %d:\n", __FILE__, __LINE__); \
      fprintf(stderr, __VA_ARGS__);                                                \
      fprintf(stderr, "\n");                                                       \
    }                                                                              \
  } while (0)                                                                      \

typedef unsigned int uint;

typedef struct pin_t {
  const char *name;
  const char *key;
  uint gpio;
  int pwm_mux_mode;
  int ain;
  int isAllocatedByDefault;
} pin_t;



/**************************************************************************************************
 * 3.GPIO
 *************************************************************************************************/

void   beagleTest       ();

uint   getGpioByName    (const char *name);
uint   getGpioByKey     (const char *key);

int    gpioIsValid      (uint gpio);
int    gpioIsExported   (uint gpio);
void   gpioExport       (uint gpio);
void   gpioUnexport     (uint gpio);

void   gpioSetDir       (uint gpio, int dir);
int    gpioGetDir       (uint gpio);

void   gpioSetValue     (uint gpio, int value);
int    gpioGetValue     (uint gpio);

void   gpioSetActiveLow (uint gpio, int value);
int    gpioGetActiveLow (uint gpio);

void   gpioSetEdge      (uint gpio, int edge);
int    gpioGetEdge      (uint gpio);

void   setLedToGpio     (uint gpio);

#ifdef __cplusplus
}
#endif

#endif // BEAGLEBONE_CONFIG_H
