
#include "Wyliodrin.h"
#include <stdio.h>

void initWyliodrin ()
{
#ifdef RASPBERRYPI
	wiringPiSetup ();
#elif ARDUINOGALILEO
	wiringSetup ();
#endif
}

void sendSignal (const char *name, double value)
{
	printf ("%s: %lf\n", name, value);
}
