#ifndef ARDUINO
#define ARDUINO

#include <wiringPi.h>

void setup ();

void loop ();

int main (int argc, char *argv[])
{
	wiringPiSetup ();
	setup ();
	while (true)
	{
		loop ();
	}
	return 0;
}

#endif
