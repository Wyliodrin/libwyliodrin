#ifndef _WYLIODRIN_H
#define _WYLIODRIN_H



/*** INCLUDES ************************************************************************************/

#include <stdlib.h>

#include <Wyliodrin/signals.h>
#include <Wyliodrin/wiring.h>
#include <Wyliodrin/lcd.h>

/*************************************************************************************************/



/*** API *****************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Signals initialization
 */
void wyliodrinSetup();

/**
 * Return version major
 */
int get_version_major();

/**
 * Return version minor
 */
int get_version_minor();

#ifdef __cplusplus
}
#endif

/*************************************************************************************************/



#endif /* _WYLIODRIN_H */
