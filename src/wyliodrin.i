
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
    #include "libraries/Pixy.h"
    #include "libraries/TPixy.h"
    #include "libraries/Adafruit_GFX.h"
    #include "libraries/Adafruit_PCD8544.h"
    #include "libraries/Adafruit_ST7735.h"
    #include "libraries/SparkFun_APDS9960.h"
    #include "libraries/Adafruit_LEDBackpack.h"
    #include "libraries/Adafruit_Sensor.h"
    #include "libraries/I2Cdev.h"
    #include "libraries/MPU6050.h"



    #include "libraries/Adafruit_BMP085_U.h"
    #include "libraries/LiquidCrystal.h"
    #include "libraries/TM1637.h"
    #include "libraries/LED_Bar.h"
    #include "libraries/CircularLED.h"
    #include "libraries/SeeedOLED.h"
    #include "libraries/HTU21D.h"
    #include "libraries/UniPi.h"
%}

%init %{
    //Adding mraa_init() to the module initialisation process
    wyliodrinSetup();
%}

#if defined(RASPBERRYPI)
%include "wiring/raspberrypi.h"
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
%include "libraries/Pixy.h"
%include "libraries/TPixy.h"
%include "libraries/Adafruit_GFX.h"
%include "libraries/Adafruit_PCD8544.h"
%include "libraries/Adafruit_ST7735.h"
%include "libraries/Adafruit_LEDBackpack.h"
%include "libraries/SparkFun_APDS9960.h"
%include "libraries/Adafruit_Sensor.h"
%include "libraries/Adafruit_BMP085_U.h"
%include "libraries/HTU21D.h"
%include "libraries/I2Cdev.h"
%include "libraries/MPU6050.h"
%include "libraries/LiquidCrystal.h"
%include "libraries/TM1637.h"
%include "libraries/LED_Bar.h"
%include "libraries/CircularLED.h"
%include "libraries/SeeedOLED.h"
%include "libraries/UniPi.h"

%template(Pixy) TPixy<LinkSPI>;
