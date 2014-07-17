/**************************************************************************************************
 *  
 * This file extends the definitions of the GPIO configuration functions for the UDOO board
 * Based on https://github.com/UDOOboard/Kernel_Unico/tree/master/Documentation
 *  and on https://github.com/cmuartfab/Udoo/blob/master/puredata/gpio/gpio.c
 * Follow "Wyliodrin Coding Style Convention" at:
 *  https://docs.google.com/document/d/14zRCX1KIwvQ1qEzWBVH-We0CkQmd5-kZb81bzvbIQKY/edit
 *
 * CONTENT:
 * 1. UDOO Pin Table
 * 2. GPIO functions extend definitions
 *
 *************************************************************************************************/


#ifdef UDOO

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "udooConfig.h"



/**************************************************************************************************
 * 1. UDOO Pin Table
 * Based on http://udoo.org/download/files/pinout/UDOO_pinout_alternate_table.pdf
 *  and on http://www.udoo.org/ProjectsAndTutorials/linux-gpio-manipulation/?portfolioID=1394
 *************************************************************************************************/



udooPin_t pinTable[] = {

/*   Bank  |   Key   |    name    | ardFunction |  gpio  */
    {"J17",  "0",       "URXD/PA8",    "RX0",       116 },
    {"J17",  "1",       "UTXD/PA9",    "TX0",       112 },
    {"J17",  "2",       "PB25",        "PWM2",       20 },
    {"J17",  "3",       "PC28",        "PWM3",       16 },
    {"J17",  "4",       "PA29-PC26",   "PWM4",       17 },
    {"J17",  "5",       "PC25",        "PWM5",       18 },
    {"J17",  "6",       "PC24",        "PWM6",       41 },
    {"J17",  "7",       "PC23",        "PWM7",       42 }, 

    {"J21",  "8",       "PC22",        "PWM8",       21 },
    {"J21",  "9",       "PC21",        "PWM9",       19 },
    {"J21",  "10",      "PB7",         "PWM10",       1 }, 
    {"J21",  "11",      "PD7",         "PWM11",       9 },
    {"J21",  "12",      "PD8",         "PWM12",       3 },
    {"J21",  "13",      "PB27",        "PWM13",      40 },
    {"J21",  "GND",     "NoName",      "GND",         0 },
    {"J21",  "AREF",    "NoName",      "AREF",        0 },
    {"J21",  "SDA1",    "PA17",        "SDA1",        0 },
    {"J21",  "SCL1",    "PA18",        "SCL1",        0 },

    {"J5",   "14",      "URXD/PA8",    "SCL",       150 },
    {"J5",   "15",      "UTXD/PA9",    "SDA",       162 },
    {"J5",   "16",      "PB25",        "RX1",       160 },
    {"J5",   "17",      "PC28",        "TX1",       161 },
    {"J5",   "18",      "PA29-PC26",   "RX2",       158 }, 
    {"J5",   "19",      "PC25",        "TX2",       159 },
    {"J5",   "20",      "PC24",        "RX3",        92 },
    {"J5",   "21",      "PC23",        "TX3",        85 },

    {"J8",   "5V",      "PB26",        "5V",          0 },
    {"J8",   "5V",      "PA14",        "5V",          0 },
    {"J8",   "22",      "PA15",        "PIN22",     123 }, 
    {"J8",   "23",      "PD0",         "PIN23",     124 },
    {"J8",   "24",      "PD1",         "PIN24",     125 },
    {"J8",   "25",      "PD2",         "PIN25",     126 },
    {"J8",   "26",      "PD3",         "PIN26",     127 },
    {"J8",   "27",      "PD6",         "PIN27",     133 },
    {"J8",   "28",      "PD9",         "PIN28",     134 },
    {"J8",   "29",      "PA7",         "PIN29",     135 },
    {"J8",   "30",      "PD10",        "PIN30",     136 },
    {"J8",   "31",      "PC1",         "PIN31",     137 },
    {"J8",   "32",      "PC2",         "PIN32",     138 },
    {"J8",   "33",      "PC3",         "PIN33",     139 },
    {"J8",   "34",      "PC4",         "PIN34",     140 },
    {"J8",   "35",      "PC5",         "PIN35",     141 },
    {"J8",   "36",      "PC6",         "PIN36",     142 },
    {"J8",   "37",      "PC7",         "PIN37",     143 },
    {"J8",   "38",      "PC8",         "PIN38",      54 },
    {"J8",   "39",      "PC9",         "PIN39",     205 },
    {"J8",   "40",      "PA19",        "PIN40",      32 },
    {"J8",   "41",      "PA20",        "PIN41",      35 },
    {"J8",   "42",      "PC19",        "PIN42",      34 },
    {"J8",   "43",      "PC18",        "PIN43",      33 },
    {"J8",   "44",      "PC17",        "PIN44",     101 },
    {"J8",   "45",      "PC16",        "PIN45",     144 },
    {"J8",   "46",      "PC15",        "PIN46",     145 },
    {"J8",   "47",      "PC14",        "PIN47",      89 },
    {"J8",   "48",      "PC13",        "PIN48",     105 },
    {"J8",   "49",      "PC12",        "PIN49",     104 },
    {"J8",   "50",      "PB21",        "PIN50",      57 },
    {"J8",   "51",      "PB14",        "PIN51",      56 },
    {"J8",   "52",      "NoName",      "PIN52",      55 },
    {"J8",   "53",      "NoName",      "PIN53",      88 },
    {"J8",   "GND",     "NoName",      "GND",         0 },
    {"J8",   "GND",     "NoName",      "GND",         0 },
    
    {"J23",  "NoName",  "NoName",      "NOT USED",    0 },
    {"J23",  "IOREF",   "NoName",      "IOREF",       0 },
    {"J23",  "RES",     "NRSTB",       "RESET",       0 },
    {"J23",  "3.3V",    "NoName",      "3.3V",        0 },
    {"J23",  "5V",      "NoName",      "5V",          0 },
    {"J23",  "GND",     "NoName",      "GND",         0 },
    {"J23",  "GND",     "NoName",      "GND",         0 },
    {"J23",  "VIN",     "NoName",      "VIN",         0 },

    {"J11",  "A0",      "PA16",        "A0",          0 },
    {"J11",  "A1",      "PA24",        "A1",          0 }, 
    {"J11",  "A2",      "PA23",        "A2",          0 },
    {"J11",  "A3",      "PA22",        "A3",          0 },
    {"J11",  "A4",      "PA6",         "A4",          0 },
    {"J11",  "A5",      "PA4",         "A5",          0 },
    {"J11",  "A6",      "PA3",         "A6",          0 },
    {"J11",  "A7",      "PA2",         "A7",          0 },

    {"J15",  "A8",      "PB17",        "A8",          0 },
    {"J15",  "A9",      "PB18",        "A9",          0 },
    {"J15",  "A10",     "PB19",        "A10",         0 },
    {"J15",  "A11",     "PB20",        "A11",         0 },
    {"J15",  "DAC0",    "PB15",        "DAC0",        0 },
    {"J15",  "DAC1",    "PB16",        "DAC1",        0 },
    {"J15",  "CANRX",   "PA0",         "CANRX",       0 },
    {"J15",  "CANTX",   "PA1",         "CANTX",       0 }};



/**************************************************************************************************
 * 2. GPIO functions [extend definitions]
 *************************************************************************************************/

/**
 * There are a few functions here that I took from the beagleboneConfig file after discussion
 * with Matei. These functions can be used for the UDOO board, too.
 * We decided not to make a generic "common" library just for the moment.
 */


/**
 * "Hello world!" function, used for testing the correctness of the install
 */

void boardTest(const char* message) 
{
    printf("%s\n", message);
}

/**
 * Given some atributes of some pin, these 3 functions return the related gpio number
 * If the gpio returned is 0, that means there is no gpio for that pin
 * In case of -1 returned, the given name doesn't exist
 */

byte getGpioByName (const char* name)
{   
    int i;
    udooPin_t *tmpTable = pinTable;
    for (i = 0; i < (sizeof(pinTable) / sizeof(pinTable[0])); i++) {
        if (strcmp(tmpTable->name, name) == 0)
            return tmpTable->gpio;
        tmpTable++;
    }
    debug("There is no pin on the board named %s", name);
    return -1;
}

byte getGpioByArdFunc (const char* ardFunction)
{
    int i;
    udooPin_t *tmpTable = pinTable;
    for (i = 0; i < (sizeof(pinTable) / sizeof(pinTable[0])); i++) {
        if (strcmp(tmpTable->ardFunction, ardFunction) == 0)
            return tmpTable->gpio;
        tmpTable++;
    }
    debug("There is no pin on the board with %s Arduino Function", ardFunction);
    return -1;
}

byte getGpioByKey (const char* key)
{
    int i;
    udooPin_t *tmpTable = pinTable;
    for (i = 0; i < (sizeof(pinTable) / sizeof(pinTable[0])); i++) {
        if (strcmp(tmpTable->key, key) == 0)
            return tmpTable->key;
        tmpTable++;
    }
    debug("There is no pin on the board with key %s", key);
    return -1;
}

byte gpioIsValid (byte gpio)
{
    if (gpio == 0)
        return 0;
    int i;
    udooPin_t *tmpTable = pinTable;
    for (i = 0; i < (sizeof(pinTable) / sizeof(pinTable[0])); i++) {
        if (tmpTable->gpio == gpio)
            return 1;
    }
    return 0;
}

byte gpioIsExported (byte gpio)
{
    char buffer[64];
    snprintf(buffer, 64, GPIO_FILE_PREFIX "gpio%d/value", gpio);
    int canOpenFile = open(buffer, O_WRONLY);
    if (canOpenFile == -1)
        return 0;
    close(canOpenFile);
    return 1;
}

/**
 * Function that set a gpio direction to input if possible
 * Returns 0 if succeeded
 */
 
int gpioSetDirInput (byte gpio)
{
    if (!gpioIsValid(gpio)) {
        debug("Can't set direction for gpio no: %d [INVALID]", gpio);
        return EINVAL;
    } else if (!gpioIsExported(gpio)) {
        debug("Can't set direction for gpio: %d [UNEXPORTED]", gpio);
        return EINVAL;
    } else {
        char buffer[64];
        snprintf(buffer, 64, GPIO_FILE_PREFIX "gpio%d/direction", gpio);
        int fo = open(buffer, O_WRONLY);
        write(fo, GPIOF_DIR_IN, sizeof(GPIOF_DIR_IN));
        close(fo);
        return 0;
    }
}

#ifdef __cplusplus
}
#endif

#endif /* UDOO */