
#include "Wyliodrin.h"
#include <stdio.h>
#include "libinetsocket.h"
#include <string.h>
#include <stdlib.h>

static int sock = -1;

void initWyliodrin ()
{
#ifdef RASPBERRYPI
	wiringPiSetup ();
#elif ARDUINOGALILEO
	wiringSetup ();
#endif
}

void connectWyliodrin ()
{
	if (sock == -1)
	{
		// printf ("connecting %s to %s\n", getenv ("wyliodrin_id"), getenv ("wyliodrin_port"));
		sock = create_inet_stream_socket("127.0.0.1",getenv ("wyliodrin_port"),LIBSOCKET_IPv4,0);
		if (sock >= 0)
		{
			char id[1000];
			sprintf (id, "id %s\n", getenv ("wyliodrin_id"));
			if (write (sock, id, strlen (id))<0) sock = -1;
		}

		// For the safe programming's sake, here should be an else error clause
		// And it should be an error clause after write also
		// I'll put them if I get the OK from Alex and Ioana
	}
}

void sendSignal (const char *name, double value)
{
	// printf ("%s: %lf\n", name, value);
	connectWyliodrin ();
	if (sock >=0)
	{
		char data[1000];
		sprintf (data, "%s %lf\n", name, value);
		if (write (sock, data, strlen (data))<0) sock = -1;
		// Else error clause
	}

	// And and else error clause here :D
}

char *test ()
{
	char *message = (char*) malloc (20 * sizeof(char));
	sprintf(message, "It Works!");
	return message;
}