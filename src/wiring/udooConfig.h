/**************************************************************************************************
 * This header file contains declarations of the GPIO configuration functions for the UDOO Board
 * Follow "Wyliodrin Coding Style Convention" at: 
 *     https://docs.google.com/document/d/14zRCX1KIwvQ1qEzWBVH-We0CkQmd5-kZb81bzvbIQKY/edit
 * 
 *
 * CONTENT:
 * 1. Constants & Flags
 * 2. GPIO
 * 3. Debug
 *************************************************************************************************/


#ifndef __UDOO_CONFIG_H__
#define __UDOO_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************************************************
 * 1. Constants
 *************************************************************************************************/


#define GPIO_FILE_PREFIX "/sys/class/gpio/"
#define GPIO_FILE_EXPORT GPIO_FILE_PREFIX "export"

// Direction
#define GPIOF_DIR_IN  "in"     // to configure direction as input
#define GPIOF_DIR_OUT "out"    // to configure direction as output 

// Values
#define HIGH 1
#define LOW  0

#define GPIOF_INIT_LOW  LOW    // as output, set initial level to LOW
#define GPIOF_INIT_HIGH HIGH   // as input, set initial level to HIGH

// Edge
enum whatEdge {
	NONE,
	RISING,
	FALLING, 
	BOTH
};

typedef unsigned char byte;

typedef struct {
	const char *bank;
	const char *key;          // the pin as it appears on the board
	const char *name;
	const char *ardFunction;  // Arduino Function
	byte gpio;
} udooPin_t;



/**************************************************************************************************
 * 2. GPIO
 *************************************************************************************************/


void boardTest          (const char* message);

byte getGpioByName      (const char* name);

byte getGpioByArdFunc   (const char* ardFunction);

byte getGpioByKey       (const char* key);

// test if some number could reference a correct GPIO.
byte gpioIsValid        (byte gpio);

byte gpioIsExported     (byte gpio);

// set as input or output, returning 1 or negative errno
int gpioSetDirInput     (byte gpio);
int gpioSetDirOutput    (byte gpio, const char* value);

// get the pin direction
byte gpioGetDir         (byte gpio);

int gpioGetValue        (byte gpio);
void gpioSetValue       (byte gpio, byte value);

void gpioExport         (byte gpio); 
void gpioUnexport       (byte gpio);

void gpioSetEdge        (byte gpio, byte edge);
byte gpioGetEdge        (byte gpio);

void gpioSetActiveLow   (byte gpio, byte value);
byte gpioGetActiveLow   (byte gpio);

// map GPIO numbers to IRQ numbers
int gpioToIrq           (byte gpio);

// map IRQ numbers to IRQ numbers
int irqToGpio           (byte gpio);



/**************************************************************************************************
 * 3. Debug
 *************************************************************************************************/

/**
 * Took this from Matei. It's a nice macro that does all the work good.
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


#endif /* __UDOO_CONFIG_H__ */