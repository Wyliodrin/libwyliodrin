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

#define MAX_BUF        64
#define SYSFS_GPIO_DIR "/sys/class/gpio"

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

typedef enum {
  INPUT  = 0,
  OUTPUT = 1
} pin_direction_t;

typedef enum {
  LOW  = 0,
  HIGH = 1
} pin_value_t;

typedef enum {
  NONE    = 0,
  RISING  = 1,
  FALLING = 2,
  BOTH    = 3,
} edge_t;

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

int    gpioIsExported   (uint gpio);
void   gpioExport       (uint gpio);
void   gpioUnexport     (uint gpio);

void   gpioSetDir       (uint gpio, pin_direction_t dir);
int    gpioGetDir       (uint gpio);

void   gpioSetValue     (uint gpio, pin_value_t value);
int    gpioGetValue     (uint gpio);

void   gpioSetActiveLow (uint gpio, int value);
int    gpioGetActiveLow (uint gpio);

void   gpioSetEdge      (uint gpio, edge_t edge);
edge_t gpioGetEdge      (uint gpio);



#ifdef __cplusplus
}
#endif

#endif // BEAGLEBONE_CONFIG_H
