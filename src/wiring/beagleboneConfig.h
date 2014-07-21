/**************************************************************************************************
 * Author: Razvan Madalin MATEI <matei.rm94@gmail.com>
 *
 * This file contains the declarations of the GPIO configuration functions for the BeagleBone Black
 *
 * CONTENT:
 * 1.Constants
 * 2.General
 * 3.Device Tree
 * 4.GPIO
 * 5.User LEDs
 * 6.PWM
 *************************************************************************************************/

#ifndef BEAGLEBONE_CONFIG_H
#define BEAGLEBONE_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif



/**************************************************************************************************
 * 1.Constants
 *************************************************************************************************/

#define MAX_BUF 64

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define SYSFS_LEDS_DIR "/sys/class/leds"
#define SYSFS_PWM_DIR  "/sys/class/pwm"

// Direction
#define INPUT  0
#define OUTPUT 1

// Value
#define LOW  0
#define HIGH 1

// Edge
#define NONE    0
#define RISING  1
#define FALLING 2
#define BOTH    3

// Trigger
#define NONE        0
#define NAND_DISK   1
#define MMC0        2
#define MMC1        3
#define TIMER       4
#define ONESHOT     5
#define HEARTBEAT   6
#define BACKLIGHT   7
#define GPIO        8
#define CPU0        9
#define DEFAULT_ON 10
#define TRANSIENT  11

#define isLed(gpio) (53 <= (gpio) && (gpio) <= 56)

typedef unsigned char byte;

typedef struct {
  const char *name;
  const char *key;
  byte gpio;
  byte pwmMuxMode;
  byte ain;
  byte isAllocatedByDefault;
} pin_t;

typedef enum {
  SUCCESS = 0,
  ERROR   = 99
} result_t;



/**************************************************************************************************
 * 2.General
 *************************************************************************************************/

#define DEBUG 1
#define debug(...)                                                                 \
  do {                                                                             \
    if (DEBUG) {                                                                   \
      fprintf(stderr, "Debug alert in file %s at line %d:\n", __FILE__, __LINE__); \
      fprintf(stderr, __VA_ARGS__);                                                \
      fprintf(stderr, "\n");                                                       \
      perror(NULL);                                                                \
    }                                                                              \
  } while (0)                                                                      \

result_t buildPath (const char *partialPath, 
                    const char *prefix, 
                    char *fullPath, 
                    size_t fullPathLen);



/**************************************************************************************************
 * 3.Device Tree
 *************************************************************************************************/

result_t loadDeviceTree   (const char *name);
result_t unloadDeviceTree (const char *name);



/**************************************************************************************************
 * 4.GPIO
 *************************************************************************************************/

void   beagleTest       ();

byte   getGpioByName    (const char *name);
byte   getGpioByKey     (const char *key);

byte   gpioIsValid      (byte gpio);
byte   gpioIsExported   (byte gpio);

void   gpioExport       (byte gpio);
void   gpioUnexport     (byte gpio);

void   gpioSetDir       (byte gpio, byte dir);
byte   gpioGetDir       (byte gpio);

void   gpioSetValue     (byte gpio, byte value);
byte   gpioGetValue     (byte gpio);

void   gpioSetActiveLow (byte gpio, byte value);
byte   gpioGetActiveLow (byte gpio);

void   gpioSetEdge      (byte gpio, byte edge);
byte   gpioGetEdge      (byte gpio);



/**************************************************************************************************
 * 5.User LEDs
 *************************************************************************************************/

void   ledSetTrigger (byte gpio, byte trigger);

void   ledSetValue   (byte gpio, byte value);
byte   ledGetValue   (byte gpio);

void   ledReset      (byte gpio);



/**************************************************************************************************
 * 6.PWM
 *************************************************************************************************/
void pwmStart   (const char *key, float duty, float freq, int polarity);
void pwmDisable (const char *key);

void pwmSetFreq (const char *key, float freq);
void pwmSetDuty (const char *key, float duty);

void pwmCleanup ();



#ifdef __cplusplus
}
#endif

#endif // BEAGLEBONE_CONFIG_H
