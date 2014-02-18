#ifndef ARDUINO
#define ARDUINO

#include <wiringPi.h>

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
