#pragma once

abc
#ifdef BEAGLEBONE

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * This file contains the declarations of the configuration functions for the
 * BeagleBone Black.
 *
 * CONTENT:
 * 1.Defines
 * 2.Pin Configuration
 * 3.Helper functions
 *****************************************************************************/



/******************************************************************************
 * 1.Defines
 *****************************************************************************/

#define INPUT  0
#define OUTPUT 1

#define SYSFS_GPIO_DIR "/sys/class/gpio"

// If they'll like this macro, I'll move it in a more generic file
#define DEBUG 1
#define debug(...)                                                           \
  do {                                                                       \
    if (DEBUG) {                                                             \
      fprintf(stderr, "Error in file %s at line %d:\n", __FILE__, __LINE__); \
      fprintf(stderr, __VA_ARGS__);                                          \
      fprintf(stderr, "\n");                                                 \
    }                                                                        \
  } while (0)                                                                \



/******************************************************************************
 * 2.Pin Configuration
 *****************************************************************************/

/**
 * Pin Structure
 */
typedef struct pin_t {
  const char *name;
  const char *key;
  unsigned int gpio;
  int pwm_mux_mode;
  int ain;
  int isAllocatedByDefault;
} pin_t;

/**
 * Pins configuration table for the BeagleBone Black
 */
pin_t pinTable[];



/******************************************************************************
 * 3.Helper functions
 * I should find a smarter name for this block.
 *****************************************************************************/

int getGpioByName (const char *name);
int getGpioByKey  (const char *key);



#ifdef __cplusplus
}
#endif

#endif // BEAGLEBONE
