
%module(docstring="Python interface to libwyliodrin") wyliodrin

%{
    #include <Wyliodrin.h>
    #include <Wyliodrin/Print.h>
    #include <Wyliodrin/Wire.h>
    #include <Wyliodrin/Servo.h>
    #include <Wyliodrin/Stepper.h>
    #include <Wyliodrin/rgb_lcd.h>
%}

%init %{
    //Adding maa_init() to the module initialisation process
    wyliodrinSetup();
%}

%include "../../src/Wyliodrin.h"
%include "../../src/wiring/wiring.h"
%include "../../src/wiring/binary.h"
%include "../../src/wiring/BitsAndBytes.h"
%include "../../src/signals/signals.h"
%include "../../src/libraries/lcd.h"
%include "../../src/libraries/Print.h"
%include "../../src/libraries/Stream.h"
%include "../../src/libraries/Wire.h"
%include "../../src/libraries/Servo.h"
%include "../../src/libraries/Stepper.h"
%include "../../src/libraries/rgb_lcd.h"
