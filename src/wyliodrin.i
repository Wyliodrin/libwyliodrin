%module(docstring="Python interface to libwyliodrin") wyliodrin

%include "stdint.i"

%{
    #include "setup.h"
    #include "signals/signals.h"
    #include "wiring/wiring.h"
    #include "libraries/lcd.h"
    #include "libraries/Print.h"
    #include "libraries/SPI.h"
    #include "libraries/Wire.h"
    #include "libraries/Servo.h"
    #include "libraries/Stepper.h"
    #include "libraries/rgb_lcd.h"
    #include "libraries/I2Cdev.h"
    #include "libraries/LiquidCrystal.h"
%}

%init %{
    wyliodrinSetup();
%}

#if defined(RASPBERRYPI)
    %include "wiring/raspberrypi.h"
#endif

#if defined(REDPITAYA)
    %include "wiring/red-pitaya.h"
#endif

%include "wiring/wiring.h"
%include "wiring/binary.h"
%include "wiring/BitsAndBytes.h"
%include "signals/signals.h"
%include "libraries/lcd.h"
%include "libraries/Print.h"
%include "libraries/Stream.h"
%include "libraries/SPI.h"
%include "libraries/Wire.h"
%include "libraries/Servo.h"
%include "libraries/Stepper.h"
%include "libraries/rgb_lcd.h"
%include "libraries/I2Cdev.h"
%include "libraries/LiquidCrystal.h"
