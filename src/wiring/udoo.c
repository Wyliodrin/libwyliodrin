/**************************************************************************************************
 * This file contains the functions available on UDOO pin headers.
 * See documentation here: 
 *      http://arduino.cc/en/Reference/HomePage
 *      file:///home/andrei/Desktop/UDOO_Starting_Manual_beta0.4_11_28_2013.pdf
 *
 * Follow "Wyliodrin Coding Style Convention" at:
 *      https://docs.google.com/document/d/14zRCX1KIwvQ1qEzWBVH-We0CkQmd5-kZb81bzvbIQKY/edit
 * 
 * CONTENT:
 * 1.  General configuration
 * 2.  Digital I/O
 * 3.  Analog I/O
 * 3.  Advanced I/O
 * 4.  Time
 * 5.  Communication (Serial, Stream)
 * 6.  UARTs: uart1, uart3, uart4, uart5
 * 7.  SD1
 * 8.  SPIs: spi1, spi2, spi5
 * 9.  I2C1
 * 10. Spdif
 * 11. Timer capture
 * 12. Timer compare
 * 13. WATCHDOG FUNCTIONALITIES: watchdog reset, watchdog out
 * 14. Clock out
 * 15. PWMs: pwm1, pwm2, pwm3, pwm4
 * 16. Digital audio 
 *************************************************************************************************/

#ifdef UDOO

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "wiring.h"
#include "udooConfig.h"


/**************************************************************************************************
 * 2. Digital I/O
 *************************************************************************************************/

/**
 * Configures the specified pin to behave either as an input or an output
 * pin: the number of the pin whose mode you wish to set (gpio pin)
 * mode: INPUT or OUTPUT
 */

void pinMode (int pin, int mode) 
{
    if (mode != INPUT && mode != OUTPUT) {
        debug("Mode can be either INPUT or OUTPUT");
        return;
    }
    gpioExport(pin);
    if (mode == INPUT) {
        gpioSetDirInput(pin);
    } else {
        gpioSetDirOutput(pin);
    }
}

/**
 * Write a HIGH or a LOW value to a pin
 * pin: the pin number (gpio pin)
 * value: HIGH or LOW
 */

void digitalWrite (int pin, int value)
{
    gpioSetValue(pin, value);
}

/** 
 * Reads the value from a specified digital pin, either HIGH or LOW
 * pin: the number of the digital pin you want to read
 */

int digitalRead (int pin)
{
    // See defined values in udooConfig.h for invalid/unexported pin that can be returned 
    return gpioGetValue(pin);
}

#ifdef __cplusplus
}
#endif

#endif /* UDOO */
