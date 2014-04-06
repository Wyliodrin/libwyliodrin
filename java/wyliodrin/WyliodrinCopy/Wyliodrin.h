#ifndef WYLIODRIN_H
#define WYLIODRIN_H

#ifdef RASPBERRYPI
#include <wiringPi.h>
#elif ARDUINOGALILEO
#include <Arduino.h>
#endif

void initWyliodrin ();
void sendSignal (const char *name, double value);
char *test (); // This functions returns the string "It Works!";

#endif

