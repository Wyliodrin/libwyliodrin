
#ifndef ARDUINO__H
#define ARDUINO__H

#include <Wyliodrin.h>
#include <stdlib.h>
#include <string.h>

#define PROGMEM

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

// main fucntion for Arduino

#ifdef __cplusplus
extern "C" {
#endif

void setup ();
void loop ();


#ifdef __cplusplus
}
#endif

int main ()
{
	wyliodrinSetup ();
	setup ();
	for (;;)
	{
		loop ();
	}
}

#endif
