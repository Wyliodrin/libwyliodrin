
#ifndef ARDUINO__H
#define ARDUINO__H

#include <Wyliodrin.h>

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
