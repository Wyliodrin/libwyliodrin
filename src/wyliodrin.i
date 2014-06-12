
%module(docstring="Python interface to libwyliodrin") wyliodrin

%{
    #include <Wyliodrin.h>
%}

%init %{
    //Adding maa_init() to the module initialisation process
    wyliodrinSetup();
%}

%include "../../src/Wyliodrin.h"
%include "../../src/wiring/wiring.h"
%include "../../src/signals/signals.h"
%include "../../src/libraries/lcd.h"
