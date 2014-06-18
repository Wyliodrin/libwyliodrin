
%module(docstring="Python interface to libwyliodrin") wyliodrin

%{
    #include "setup.h"
    #include "signals/signals.h"
    #include "wiring/wiring.h"
    #include "libraries/lcd.h"
    #include "libraries/Print.h"
    #include "libraries/Wire.h"
    #include "libraries/Servo.h"
    #include "libraries/Stepper.h"
    #include "libraries/rgb_lcd.h"
%}

%init %{
    //Adding maa_init() to the module initialisation process
    wyliodrinSetup();
%}

%include "wiring/wiring.h"
%include "wiring/binary.h"
%include "wiring/BitsAndBytes.h"
%include "signals/signals.h"
%include "libraries/lcd.h"
%include "libraries/Print.h"
%include "libraries/Stream.h"
%include "libraries/Wire.h"
%include "libraries/Servo.h"
%include "libraries/Stepper.h"
%include "libraries/rgb_lcd.h"
