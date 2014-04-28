#ifndef WYLIODRIN_H
#define WYLIODRIN_H

#ifdef RASPBERRYPI
	#include <wiringPi.h>
#elif ARDUINOGALILEO
	#include <Arduino.h>
#endif

void initWyliodrin ();
void sendSignal    (const char *name, double value);

#endif // WYLIODRIN_H

