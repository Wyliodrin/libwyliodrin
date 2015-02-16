/**************************************************************************************************
 * Author: Razvan Madalin MATEI <matei.rm94@gmail.com>
 *
 * This file contains the declarations of all configuration functions for the BeagleBone Black.
 *
 * CONTENT:
 * 1.Constants
 * 2.General
 * 3.Device Tree
 * 4.GPIO
 * 5.User LEDs
 * 6.PWM
 * 7.AIN
 * 8.I2C
 * 9.SPI
 *************************************************************************************************/

#ifndef BEAGLEBONE_CONFIG_H
#define BEAGLEBONE_CONFIG_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif



/**************************************************************************************************
 * 1.Constants
 *************************************************************************************************/

#define MAX_BUF 128
#define NO_PINS sizeof(pinTable) / sizeof(pinTable[0])

// Paths
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define SYSFS_LEDS_DIR "/sys/class/leds"

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

// I2C Buses
#define I2C_0 0
#define I2C_1 1
//#define I2C_2 2
#define MAX_I2C_PINS 2

// SPI
#define SPI_0 0
#define SPI_1 1

typedef unsigned char byte;
typedef unsigned int  uint;
typedef unsigned long ulong;

typedef struct pin_t {
  const char *name;
  const char *key;
  byte gpio;
  byte pwmMuxMode;
  byte ain;
  byte isAllocatedByDefault;
} pin_t;

typedef struct pwmNode_t {
  const char *key;
  const char *pathPwmTest;

  struct pwmNode_t *next;
} pwmNode_t;

typedef enum {
  ERROR   = 0,
  SUCCESS = 1
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
  } while(0)                                                                       \

void        beagleTest    ();

result_t    buildPath     (const char *dirPath, const char *prefix, char *fullPath, int pathLen);

byte        getGpioByPos  (const byte pos);
byte        getGpioByName (const char *name);
byte        getGpioByKey  (const char *key);

const char* getNameByGpio (const byte gpio);
const char* getKeyByGpio  (const byte gpio);



/**************************************************************************************************
 * 3.Device Tree
 *************************************************************************************************/

result_t loadDeviceTree   (const char *name);
result_t unloadDeviceTree (const char *name);



/**************************************************************************************************
 * 4.GPIO
 *************************************************************************************************/

bool        gpioIsValid      (const byte gpio);
bool        gpioIsExported   (const byte gpio);

result_t    gpioExport       (const byte gpio);
result_t    gpioUnexport     (const byte gpio);

result_t    gpioSetDir       (const byte gpio, const byte dir);
byte        gpioGetDir       (const byte gpio);

result_t    gpioSetValue     (const byte gpio, const byte value);
byte        gpioGetValue     (const byte gpio);

result_t    gpioSetActiveLow (const byte gpio, const byte value);
byte        gpioGetActiveLow (const byte gpio);

result_t    gpioSetEdge      (const byte gpio, const byte edge);
byte        gpioGetEdge      (const byte gpio);



/**************************************************************************************************
 * 5.User LEDs
 *************************************************************************************************/

bool     ledIsValid    (const byte gpio);

result_t ledSetTrigger (const byte gpio, const byte trigger);

result_t ledSetValue   (const byte gpio, const byte value);
byte     ledGetValue   (const byte gpio);

result_t ledReset      (const byte gpio);



/**************************************************************************************************
 * 6.PWM
 *************************************************************************************************/

void       pwmInit        ();
bool       pwmIsValid     (const byte gpio);
bool       pwmIsEnabled   (const byte gpio);
pwmNode_t* pwmGetPin      (const char *key);

result_t   pwmEnable      (const char *key);
result_t   pwmDisable     (const char *key);

result_t   pwmSetPeriod   (const char *key, const ulong period);
ulong      pwmGetPeriod   (const char *key);

result_t   pwmSetDuty     (const char *key, const ulong duty);
ulong      pwmGetDuty     (const char *key);

result_t   pwmSetPolarity (const char *key, const byte polarity);
byte       pwmGetPolarity (const char *key);

result_t   pwmSetRun      (const char* key, const byte run);
byte       pwmGetRun      (const char* key);



/**************************************************************************************************
 * 7.AIN
 *************************************************************************************************/

void ainInit     ();
bool ainIsValid  (const byte gpio);

int  ainGetValue (const byte gpio);



/**************************************************************************************************
 * 8.I2C
 *************************************************************************************************/

result_t i2cEnableI2C2 ();
int      i2cOpenBus    (byte bus);
result_t i2cInitComm   (int fd, int address);
result_t i2cRead       (int fd, char *data, int length);
result_t i2cWrite      (int fd, char *data, int length);



/**************************************************************************************************
 * 9.SPI
 *************************************************************************************************/
result_t spiEnableSPIDEV0  ();
result_t spiEnableSPIDEV1  ();
int      spiOpenBus        (byte bus, byte chipselect);

result_t spiSetMode        (int fd, byte mode);
byte     spiGetMode        (int fd);

result_t spiSetBitsPerWord (int fd, byte bits);
byte     spiGetBitsPerWord (int fd);

result_t spiSetMaxSpeed    (int fd, uint speed);
uint     spiGetMaxSpeed    (int fd);


#ifdef __cplusplus
}
#endif

#endif // BEAGLEBONE_CONFIG_H
