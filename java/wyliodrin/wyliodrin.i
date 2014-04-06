%module wyliodrin
	%{
		#include <Wyliodrin.h>
	%}
	 
void initWyliodrin ();
void sendSignal (const char *name, double value);
char *test ();