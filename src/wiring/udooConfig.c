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


#ifdef UDOO

#ifdef __cplusplus
extern "C" {
#endif

#include "udooConfig.h"



/**************************************************************************************************
 * 1. UDOO Pin Table && Analog Pin Description
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


extern const analogPinDescription analogPin[] = {
    // 0 .. 53 - Digital pins
    // ----------------------
    // 0/1 - UART (Serial)
    {NO_ADC, NO_ADC},  // URXD
    {NO_ADC, NO_ADC},  // UTXD

    // 2
    {NO_ADC, NO_ADC},  // TIOA0
    {NO_ADC, NO_ADC},  // TIOA7
    {NO_ADC, NO_ADC},  // TIOB6

    // 5
    {NO_ADC, NO_ADC},  // TIOA6
    {NO_ADC, NO_ADC},  // PWML7
    {NO_ADC, NO_ADC},  // PWML6
    {NO_ADC, NO_ADC},  // PWML5
    {NO_ADC, NO_ADC},  // PWML4
    // 10
    {NO_ADC, NO_ADC},  // TIOB7
    {NO_ADC, NO_ADC},  // TIOA8
    {NO_ADC, NO_ADC},  // TIOB8

    // 13 - AMBER LED
    {NO_ADC, NO_ADC},  // TIOB0

    // 14/15 - USART3 (Serial3)
    {NO_ADC, NO_ADC},  // TXD3
    {NO_ADC, NO_ADC},  // RXD3

    // 16/17 - USART1 (Serial2)
    {NO_ADC, NO_ADC},  // TXD1
    {NO_ADC, NO_ADC},  // RXD1

    // 18/19 - USART0 (Serial1)
    {NO_ADC, NO_ADC},  // TXD0
    {NO_ADC, NO_ADC},  // RXD0

    // 20/21 - TWI1
    {NO_ADC, NO_ADC},  // TWD1 - SDA0
    {NO_ADC, NO_ADC},  // TWCK1 - SCL0

    // 22
    {NO_ADC, NO_ADC},  // PIN22
    {NO_ADC, NO_ADC},  // PIN23
    {NO_ADC, NO_ADC},  // PIN24
    {NO_ADC, NO_ADC},  // PIN25
    {NO_ADC, NO_ADC},  // PIN26
    {NO_ADC, NO_ADC},  // PIN27
    {NO_ADC, NO_ADC},  // PIN28
    {NO_ADC, NO_ADC},  // PIN29
    {NO_ADC, NO_ADC},  // PIN30
    {NO_ADC, NO_ADC},  // PIN31
    {NO_ADC, NO_ADC},  // PIN32
    {NO_ADC, NO_ADC},  // PIN33
    {NO_ADC, NO_ADC},  // PIN34
    {NO_ADC, NO_ADC},  // PIN35
    {NO_ADC, NO_ADC},  // PIN36
    {NO_ADC, NO_ADC},  // PIN37
    {NO_ADC, NO_ADC},  // PIN38
    {NO_ADC, NO_ADC},  // PIN39
    {NO_ADC, NO_ADC},  // PIN40
    {NO_ADC, NO_ADC},  // PIN41
    {NO_ADC, NO_ADC},  // PIN42
    {NO_ADC, NO_ADC},  // PIN43
    {NO_ADC, NO_ADC},  // PIN44
    {NO_ADC, NO_ADC},  // PIN45
    {NO_ADC, NO_ADC},  // PIN46
    {NO_ADC, NO_ADC},  // PIN47
    {NO_ADC, NO_ADC},  // PIN48
    {NO_ADC, NO_ADC},  // PIN49
    {NO_ADC, NO_ADC},  // PIN50
    {NO_ADC, NO_ADC},  // PIN51
    {NO_ADC, NO_ADC},  // PIN52
    {NO_ADC, NO_ADC},  // PIN53

    // 54 .. 65 - Analog pins 
    // ----------------------
    {ADC0, ADC7},      // AD0
    {ADC1, ADC6},      // AD1
    {ADC2, ADC5},      // AD2
    {ADC3, ADC4},      // AD3
    {ADC4, ADC3},      // AD4
    {ADC5, ADC2},      // AD5
    {ADC6, ADC1},      // AD6
    {ADC7, ADC0},      // AD7
    {ADC8, ADC10},     // AD8
    {ADC9, ADC11},     // AD9
    {ADC10, ADC12},    // AD10
    {ADC11, ADC13},    // AD11

    // 66/67 - DAC0/DAC1
    {ADC12, DA0},      // DAC0
    {ADC13, DA1},      // DAC1

    // 68/69 - CANRX0/CANTX0
    {ADC14, NO_ADC},   // CANRX
    {ADC15, NO_ADC},   // CANTX
};

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

#endif /* UDOO */