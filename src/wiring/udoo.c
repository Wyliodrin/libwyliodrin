/**************************************************************************************************
 * This file contains the functions available on UDOO pin headers.
 * See documentation here: 
 * 		http://arduino.cc/en/Reference/HomePage
 * 		file:///home/andrei/Desktop/UDOO_Starting_Manual_beta0.4_11_28_2013.pdf
 *
 * Follow "Wyliodrin Coding Style Convention" at:
 *  	https://docs.google.com/document/d/14zRCX1KIwvQ1qEzWBVH-We0CkQmd5-kZb81bzvbIQKY/edit
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
#include "wiring.h"
#include "udooConfig.h"


#ifdef __cplusplus
}
#endif

#endif /* UDOO */
