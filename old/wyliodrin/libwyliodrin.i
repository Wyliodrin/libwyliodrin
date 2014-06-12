%module libwyliodrin
%{
	#include <Wyliodrin.h>
%}

void sendSignal (const char *name, double value);

