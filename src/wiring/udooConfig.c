/**************************************************************************************************
 *  
 * This file extends the definitions of the GPIO configuration functions for the UDOO board
 * Based on https://github.com/UDOOboard/Kernel_Unico/tree/master/Documentation
 *  and on https://github.com/cmuartfab/Udoo/blob/master/puredata/gpio/gpio.c
 * Follow "Wyliodrin Coding Style Convention" at:
 *  https://docs.google.com/document/d/14zRCX1KIwvQ1qEzWBVH-We0CkQmd5-kZb81bzvbIQKY/edit
 *
 * CONTENT:
 * 1. UDOO Pin Table && Analog Pin Description
 * 2. GPIO functions extend definitions
 *
 *************************************************************************************************/


//#ifdef UDOO

#ifdef __cplusplus
extern "C" {
#endif

#include "udooConfig.h"



/**************************************************************************************************
 * 1. UDOO Pin Table && Analog Pin Description
 * Based on http://udoo.org/download/files/pinout/UDOO_pinout_alternate_table.pdf
 *  and on http://www.udoo.org/ProjectsAndTutorials/linux-gpio-manipulation/?portfolioID=1394
 *************************************************************************************************/



extern const udooPin_t pinTable[] = {

/*   Bank  |   Key    |      name      | ardFunction |  gpio  */
    
    // 0 .. 53 - Digital pins
    // ----------------------
    // 0/1 - UART (Serial)
    {"J17",   "PA8",    "CSI0_DAT11",   "RX0",         116 },   // URXD
    {"J17",   "PA9",    "CSI0_DAT10",   "TX0",         112 },   // UTXD

    // 2
    {"J17",   "PB25",   "SD1_CLK",      "PWM2",         20 },
    {"J17",   "PC28",   "SD1_DAT0",     "PWM3",         16 },
    {"J17",   "PA29",   "SD1_DAT1",     "PWM4",         17 },
    {"J17",   "PC25",   "SD1_CMD",      "PWM5",         18 },
    {"J17",   "PC24",   "SD4_DAT1",     "PWM6",         41 },
    {"J17",   "PC23",   "SD4_DAT2",     "PWM7",         42 }, 
    {"J21",   "PC22",   "SD1_DAT3",     "PWM8",         21 },
    {"J21",   "PC21",   "SD1_DAT2",     "PWM9",         19 },
    {"J21",   "PB7",    "GPIO_1",       "PWM10",         1 }, 
    {"J21",   "PD7",    "GPIO_9",       "PWM11",         9 },
    {"J21",   "PD8",    "GPIO_3",       "PWM12",         3 },
    {"J21",   "PB27",   "SD4_DAT0",     "PWM13",        40 },

    // 14
    {"J5",    "PA8",    "EIM_D21",      "SCL",         150 },
    {"J5",    "PA9",    "EIM_D28",      "SDA",         162 },
    {"J5",    "PB25",   "CSI0_DAT13",   "RX1",         160 },
    {"J5",    "PC28",   "CSI0_DAT12",   "TX1",         161 },
    {"J5",    "PA29",   "CSI0_DAT15",   "RX2",         158 }, 
    {"J5",    "PC25",   "CSI0_DAT14",   "TX2",         159 },
    {"J5",    "PC24",   "CSI0_DAT16",   "RX3",          92 },
    {"J5",    "PC23",   "CSI0_DAT4",    "TX3",          85 },

    // 22
    {"J8",    "PA15",   "DISP0_DAT6",   "PIN22",       123 }, 
    {"J8",    "PD0",    "DISP0_DAT7",   "PIN23",       124 },
    {"J8",    "PD1",    "DISP0_DAT8",   "PIN24",       125 },
    {"J8",    "PD2",    "DISP0_DAT9",   "PIN25",       126 },
    {"J8",    "PD3",    "DISP0_DAT10",  "PIN26",       127 },
    {"J8",    "PD6",    "DISP0_DAT11",  "PIN27",       133 },
    {"J8",    "PD9",    "DISP0_DAT12",  "PIN28",       134 },
    {"J8",    "PA7",    "DISP0_DAT13",  "PIN29",       135 },
    {"J8",    "PD10",   "DISP0_DAT14",  "PIN30",       136 },
    {"J8",    "PC1",    "DISP0_DAT15",  "PIN31",       137 },
    {"J8",    "PC2",    "DISP0_DAT16",  "PIN32",       138 },
    {"J8",    "PC3",    "DISP0_DAT17",  "PIN33",       139 },
    {"J8",    "PC4",    "DISP0_DAT18",  "PIN34",       140 },
    {"J8",    "PC5",    "DISP0_DAT19",  "PIN35",       141 },
    {"J8",    "PC6",    "DISP0_DAT20",  "PIN36",       142 },
    {"J8",    "PC7",    "DISP0_DAT21",  "PIN37",       143 },
    {"J8",    "PC8",    "EIM_A16",      "PIN38",        54 },
    {"J8",    "PC9",    "GPIO_18",      "PIN39",       205 },
    {"J8",    "PA19",   "NANDF_D0",     "PIN40",        32 },
    {"J8",    "PA20",   "NANDF_D3",     "PIN41",        35 },
    {"J8",    "PC19",   "NANDF_D2",     "PIN42",        34 },
    {"J8",    "PC18",   "NANDF_D1",     "PIN43",        33 },
    {"J8",    "PC17",   "GPIO_19",      "PIN44",       101 },
    {"J8",    "PC16",   "DISP0_DAT22",  "PIN45",       144 },
    {"J8",    "PC15",   "DISP0_DAT23",  "PIN46",       145 },
    {"J8",    "PC14",   "EIM_D25",      "PIN47",        89 },
    {"J8",    "PC13",   "KEY_ROW1",     "PIN48",       105 },
    {"J8",    "PC12",   "KEY_COL1",     "PIN49",       104 },
    {"J8",    "PB21",   "EIM_OE",       "PIN50",        57 },
    {"J8",    "PB14",   "EIM_CS1",      "PIN51",        56 },
    {"J8",    "NoKey",  "EIM_CS0",      "PIN52",        55 },
    {"J8",    "NoKey",  "EIM_D24",      "PIN53",        88 },
    
    // 54
    {"J11",   "PA16",   "NoName",       "A0",            0 },
    {"J11",   "PA24",   "NoName",       "A1",            0 }, 
    {"J11",   "PA23",   "NoName",       "A2",            0 },
    {"J11",   "PA22",   "NoName",       "A3",            0 },
    {"J11",   "PA6",    "NoName",       "A4",            0 },
    {"J11",   "PA4",    "NoName",       "A5",            0 },
    {"J11",   "PA3",    "NoName",       "A6",            0 },
    {"J11",   "PA2",    "NoName",       "A7",            0 },

    // 62
    {"J15",   "PB17",   "NoName",       "A8",            0 },
    {"J15",   "PB18",   "NoName",       "A9",            0 },
    {"J15",   "PB19",   "NoName",       "A10",           0 },
    {"J15",   "PB20",   "NoName",       "A11",           0 },
    {"J15",   "PB15",   "NoName",       "DAC0",          0 },
    {"J15",   "PB16",   "NoName",       "DAC1",          0 },
    {"J15",   "PA0",    "NoName",       "CANRX",         0 },
    {"J15",   "PA1",    "NoName",       "CANTX",         0 }};



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

int getGpioByName (const char* name)
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

int getGpioByArdFunc (const char* ardFunction)
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

int getGpioByKey (const char* key)
{
    int i;
    udooPin_t *tmpTable = pinTable;
    for (i = 0; i < (sizeof(pinTable) / sizeof(pinTable[0])); i++) {
        if (strcmp(tmpTable->key, key) == 0)
            return tmpTable->gpio;
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
        tmpTable++;
    }
    return 0;
}

byte gpioIsExported (byte gpio)
{
    char buffer[64];
    snprintf(buffer, 64, GPIO_FILE_PREFIX "gpio%d/value", gpio);
    int canOpenFile = open(buffer, O_RDONLY);
    if (canOpenFile == -1)
        return 0;
    close(canOpenFile);
    return 1;
}

/**
 * Functions that set a gpio direction to input/output if possible
 * Return 0 if succeeded and negative errNo defined in udooConfig.h contrary
 */

int gpioSetDirInput (byte gpio)
{
    if (!gpioIsValid(gpio)) {
        debug("Can't set direction for gpio no: %d [INVALID]", gpio);
        return PIN_INVALID_ERROR;
    } else if (!gpioIsExported(gpio)) {
        debug("Can't set direction for gpio: %d [UNEXPORTED]", gpio);
        return PIN_UNEXPORTED_ERROR;
    } else {
        char buffer[64];
        snprintf(buffer, 64, GPIO_FILE_PREFIX "gpio%d/direction", gpio);
        int fo = open(buffer, O_WRONLY);
        write(fo, GPIOF_DIR_IN, 3);
        close(fo);
        return 0;
    }
}

int gpioSetDirOutput (byte gpio)
{
    if (!gpioIsValid(gpio)) {
        debug("Can't set direction for gpio no: %d [INVALID]", gpio);
        return PIN_INVALID_ERROR;
    } else if (!gpioIsExported(gpio)) {
        debug("Can't set direction for gpio: %d [UNEXPORTED]", gpio);
        return PIN_UNEXPORTED_ERROR;
    } else {
        char buffer[64];
        snprintf(buffer, 64, GPIO_FILE_PREFIX "gpio%d/direction", gpio);
        int fo = open(buffer, O_WRONLY);
        write(fo, GPIOF_DIR_OUT, 4);
        close(fo);
        return 0;
    }
}

int gpioGetDir (byte gpio)
{
    if (!gpioIsValid(gpio)) {
        debug("Can't get direction for gpio no: %d [INVALID]", gpio);
        return PIN_INVALID_ERROR;
    } else if (!gpioIsExported(gpio)) {
        debug("Can't get direction for gpio: %d [UNEXPORTED]", gpio);
        return PIN_UNEXPORTED_ERROR;
    } else {
        char buffer[64];
        snprintf(buffer, 64, GPIO_FILE_PREFIX "gpio%d/direction", gpio);
        int fo = open(buffer, O_RDONLY);
        memset(buffer, 0, 64);
        read(fo, buffer, 64);
        close(fo);
        if (strncmp(buffer, GPIOF_DIR_IN, 2) == 0) {
            return INPUT;
        } else if (strncmp(buffer, GPIOF_DIR_OUT, 3) == 0)
            return OUTPUT;
    }
}

/**
 * Set/Get the gpio value
 * 0 returned for the 1st and the gpio val for the 2nd function if operation succeeded
 */

int gpioSetValue (byte gpio, byte value)
{
    if (!gpioIsValid(gpio)) {
        debug("Can't set value for gpio no: %d [INVALID]", gpio);
        return PIN_INVALID_ERROR;
    } else if (!gpioIsExported(gpio)) {
        debug("Can't set value for gpio: %d [UNEXPORTED]", gpio);
        return PIN_UNEXPORTED_ERROR;
    } else {
        char buffer[64];
        snprintf(buffer, 64, GPIO_FILE_PREFIX "gpio%d/value", gpio);
        int fo = open(buffer, O_WRONLY);
        if (value == LOW) {
            write(fo, "0", 2);
        } else if (value == HIGH) {
            write(fo, "1", 2);
        } else {
            debug("Value can be either LOW or HIGH");
            close(fo);
            return UNKNOWN_VALUE_ERROR;
        }
        close(fo);
        return 0;
    }
}

int gpioGetValue (byte gpio)
{
    if (!gpioIsValid(gpio)) {
        debug("Can't get value for gpio no: %d [INVALID]", gpio);
        return PIN_INVALID_ERROR;
    } else if (!gpioIsExported(gpio)) {
        debug("Can't get value for gpio: %d [UNEXPORTED]", gpio);
        return PIN_UNEXPORTED_ERROR;
    } else {
        char buffer[64], value;
        snprintf(buffer, 64, GPIO_FILE_PREFIX "gpio%d/value", gpio);
        int fo = open(buffer, O_RDONLY);
        read(fo, &value, 1);
        close(fo);
        if (value == '0') {
            return LOW;
        } else if (value == '1') {
            return HIGH;
        }
    }
}


/**
 * Export/Unexport a specified pin
 * Functions return 0 if operation succeeded
 */

int gpioExport (byte gpio)
{
    if (!gpioIsValid(gpio)) {
        debug("Can't export gpio no: %d [INVALID]", gpio);
        return PIN_INVALID_ERROR;
    } else if (gpioIsExported(gpio)) {
        debug("Can't export gpio no: %d [ALREADY EXPORTED]", gpio);
        return PIN_EXPORTED_ERROR;
    } else {
        char buffer[64];
        int sLength = snprintf(buffer, 64, "%d", gpio);
        int fo = open(GPIO_FILE_EXPORT, O_WRONLY);
        write(fo, buffer, sLength);
        close(fo);
        return 0;
    }
}

int gpioUnexport (byte gpio)
{
    if (!gpioIsValid(gpio)) {
        debug("Can't unexport gpio no: %d [INVALID]", gpio);
        return PIN_INVALID_ERROR;
    } else if (!gpioIsExported(gpio)) {
        debug("Can't unexport gpio no: %d [ALREADY UNEXPORTED]", gpio);
        return PIN_UNEXPORTED_ERROR;
    } else {
        char buffer[64];
        int sLength = snprintf(buffer, 64, "%d", gpio);
        int fo = open(GPIO_FILE_UNEXPORT, O_WRONLY);
        write(fo, buffer, sLength);
        close(fo);
        return 0;
    }
}

#ifdef __cplusplus
}
#endif

//#endif /* UDOO */