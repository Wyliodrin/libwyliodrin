/**************************************************************************************************
 * This header file contains declarations of the GPIO configuration functions for the UDOO Board
 * 
 * Follow "Wyliodrin Coding Style Convention" at: 
 *     https://docs.google.com/document/d/14zRCX1KIwvQ1qEzWBVH-We0CkQmd5-kZb81bzvbIQKY/edit
 * 
 * CONTENT:
 * 1. Includes 
 * 2. Constants, Flags and Structures
 * 3. GPIO
 * 4. Servo
 * 5. Debug
 *************************************************************************************************/


#ifndef __UDOO_CONFIG_H__
#define __UDOO_CONFIG_H__

// modify this in the future so the user can choose if he wants either firmata active or not
#ifndef __FIRMATA__
#define __FIRMATA__

#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************************************************
 * 1. Includes
 *************************************************************************************************/

/**
 * All the required libraries in most of cases
 * Particular libraries are included separately in the c files
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "firmata.h"

/**************************************************************************************************
 * 2. Constants, Flags and Structures
 *************************************************************************************************/


#define GPIO_FILE_PREFIX "/sys/class/gpio/"
#define GPIO_FILE_EXPORT GPIO_FILE_PREFIX "export"
#define GPIO_FILE_UNEXPORT GPIO_FILE_PREFIX "unexport"

// Direction
#define GPIOF_DIR_IN  "in"        // to configure direction as input
#define GPIOF_DIR_OUT "out"       // to configure direction as output 

// Values
#define HIGH 1
#define LOW  0

#define INPUT 0
#define OUTPUT 1

#define GPIOF_INIT_LOW  LOW       // as output, set initial level to LOW
#define GPIOF_INIT_HIGH HIGH      // as input, set initial level to HIGH

#define PIN_UNEXPORTED_ERROR -132 // pin is unexported
#define PIN_EXPORTED_ERROR -133   // pin is exported
#define PIN_INVALID_ERROR -134    // pin is invalid
#define UNKNOWN_VALUE_ERROR -135
#define NOT_ANALOG_PIN_ERROR -136
#define CLOCK_TIME_ERROR -137

#define ADC_RESOLUTION 12

#define LSBFIRST 0
#define MSBFIRST 1

// i2c stuff
#define I2C_NOCMD 0
#define I2C_SMBUS_I2C_BLOCK_MAX 32
#define MAX_UDOO_I2C_BUSES 3

// Edge
enum whatEdge {
    NONE,
    RISING,
    FALLING, 
    BOTH
};

typedef enum _EAnalogChannel {
    NO_ADC = -1,
    ADC0 = 0,
    ADC1,
    ADC2,
    ADC3,
    ADC4,
    ADC5,
    ADC6,
    ADC7,
    ADC8,
    ADC9,
    ADC10,
    ADC11,
    ADC12,
    ADC13,
    ADC14,
    ADC15,
    DA0,
    DA1
} EAnalogChannel;

typedef enum _eAnalogReference {
    AR_DEFAULT = 5,
} eAnalogReference;

// Analog Pins
static const uint8_t A0    = 54;
static const uint8_t A1    = 55;
static const uint8_t A2    = 56;
static const uint8_t A3    = 57;
static const uint8_t A4    = 58;
static const uint8_t A5    = 59;
static const uint8_t A6    = 60;
static const uint8_t A7    = 61;
static const uint8_t A8    = 62;
static const uint8_t A9    = 63;
static const uint8_t A10   = 64;
static const uint8_t A11   = 65;
static const uint8_t DAC0  = 66;
static const uint8_t DAC1  = 67;
static const uint8_t CANRX = 68;
static const uint8_t CANTX = 69;

typedef unsigned char byte;

typedef struct {
    const char *bank;
    const char *key;             // the pin as it appears on the board
    const char *name;
    const char *ardFunction;     // Arduino Function
    byte gpio;
} udooPin_t;

extern const udooPin_t pinTable[];

typedef struct s_servo {
    t_firmata *firmata;
    int pin;
} t_servo;


/**************************************************************************************************
 * 3. GPIO
 *************************************************************************************************/


void boardTest          (const char* message);

int getGpioByName      (const char* name);

int getGpioByArdFunc   (const char* ardFunction);

int getGpioByKey       (const char* key);

// test if some number could reference a correct GPIO.
byte gpioIsValid        (byte gpio);

byte gpioIsExported     (byte gpio);

// set as input or output, returning 1 or negative errno
int gpioSetDirInput     (byte gpio);
int gpioSetDirOutput    (byte gpio);

// get the pin direction
int gpioGetDir          (byte gpio);

int gpioGetValue        (byte gpio);
int gpioSetValue        (byte gpio, byte value);

int gpioExport         (byte gpio); 
int gpioUnexport       (byte gpio);

void gpioSetEdge        (byte gpio, byte edge);
byte gpioGetEdge        (byte gpio);

void gpioSetActiveLow   (byte gpio, byte value);
byte gpioGetActiveLow   (byte gpio);

// map GPIO numbers to IRQ numbers
int gpioToIrq           (byte gpio);

// map IRQ numbers to IRQ numbers
int irqToGpio           (byte gpio);



/**************************************************************************************************
 * 4. Servo
 *************************************************************************************************/

// attach the servo variable to a pin
t_servo *servo_attach (int pin);

// writes a value to the servo, controlling the shaft accordingly
int servo_write (t_servo *servo, int value);


/**************************************************************************************************
 * 4. Debug
 *************************************************************************************************/

/**
 * Took this from Matei. 
 * It's a nice macro that does all the work good.
 */

#define DEBUG 1
#define debug(...)                                                                 \
  do {                                                                             \
    if (DEBUG) {                                                                   \
      fprintf(stderr, "Debug alert in file %s at line %d:\n", __FILE__, __LINE__); \
      fprintf(stderr, __VA_ARGS__);                                                \
      fprintf(stderr, "\n");                                                       \
    }                                                                              \
  } while (0)                                                                      \



#ifdef __cplusplus
}
#endif

#endif /* __FIRMATA__ */
#endif /* __UDOO_CONFIG_H__ */
