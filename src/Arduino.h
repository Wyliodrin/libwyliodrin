
#ifndef ARDUINO__H
#define ARDUINO__H

#include <Wyliodrin.h>

// main fucntion for Arduino

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
