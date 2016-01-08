#ifndef _ARDUINO_H
#define _ARDUINO_H



/*** INCLUDES ************************************************************************************/

#include <stdlib.h>
#include <string.h>

#include <Wyliodrin.h>

/*************************************************************************************************/



#define PROGMEM /* Put this information in flash memory */



/*** ARDUINO *************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void setup();
void loop();

#ifdef __cplusplus
}
#endif

int main() {
	wyliodrinSetup();

	setup();
	for (;;) {
		loop();
	}
}

/*************************************************************************************************/



#endif /* _ARDUINO_H */
