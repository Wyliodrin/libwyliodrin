cmake_minimum_required (VERSION 2.8.8)

project (libwyliodrin)

set (CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -Wall")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# Appends the cmake/modules path to MAKE_MODULE_PATH variable.
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# Dependencies
find_package (Threads)
find_package (SWIG)
if (SWIG_FOUND)
  include (${SWIG_USE_FILE})
endif ()
find_package (PkgConfig REQUIRED)
find_package (Hiredis REQUIRED)
include_directories (${HIREDIS_INCLUDE_DIR})
find_package (Jansson REQUIRED)
include_directories (${JANSSON_INCLUDE_DIR})
pkg_check_modules (EVENT REQUIRED libevent)

# Version
set (VERSION_MAJOR 3)
set (VERSION_MINOR 4)

# Configuration file
configure_file (
  "${PROJECT_SOURCE_DIR}/src/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)

# Options
option (RASPBERRYPI "Build for the RaspberryPi."   OFF)
option (GALILEO     "Build for the Intel Galileo." OFF)
option (EDISON      "Build for the Intel Galileo." OFF)
option (BEAGLEBONE  "Build for the Beaglebone."    OFF)
option (REDPITAYA   "Build for the RedPitaya."     OFF)
option (UDOONEO     "Build for the UDOO NEO."      OFF)
option (SERVER      "Build for the Server."        OFF)

# Option management
if (GALILEO)
  add_definitions (-DARDUINOGALILEO)
  pkg_check_modules (MRAA REQUIRED mraa>=0.4.0)
  message (INFO "found libmraa version: ${MRAA_VERSION}")
  set(FLAGS "-DARDUINOGALILEO")

elseif (EDISON)
  add_definitions (-DEDISON)
  pkg_check_modules (MRAA REQUIRED mraa>=0.4.0)
  message (INFO "found libmraa version: ${MRAA_VERSION}")
  set(FLAGS "-DEDISON")

elseif (RASPBERRYPI)
  add_definitions (-DRASPBERRYPI)
  find_package (Wiringpi REQUIRED)
  set (FLAGS "-DRASPBERRYPI")
  include_directories (${WIRINGPI_INCLUDE_DIR})

elseif (BEAGLEBONE)
  add_definitions (-DBEAGLEBONE)
  set (FLAGS "-DBEAGLEBONE")

elseif (REDPITAYA)
  add_definitions (-DREDPITAYA)
  find_package (rp REQUIRED)
  set (FLAGS "-DREDPITAYA")
  include_directories (${REDPITAYA_INCLUDE_DIR})

elseif (UDOONEO)
  add_definitions (-DUDOONEO)
  set(FLAGS "-DUDOONEO")

elseif (SERVER)
  add_definitions (-DSERVER)
  set(FLAGS "-DSERVER")

else ()
  message (FATAL_ERROR "You need to choose between -DRASPBERRYPI=ON, -DGALILEO=ON, -DEDISON=ON, -DBEAGLEBONE=ON, -DREDPITAYA=ON, -DUDOONEO=ON or -DSERVER=ON")
endif ()


add_definitions ("-Wall -DDEBUG")

include_directories (${PROJECT_BINARY_DIR})

# Sources
set (wyliodrin_LIB_SRCS
  # Core
  ${PROJECT_SOURCE_DIR}/src/version.c
  ${PROJECT_SOURCE_DIR}/src/setup.c
  ${PROJECT_SOURCE_DIR}/src/wiring/BitsAndBytes.c
  ${PROJECT_SOURCE_DIR}/src/signals/signals.c
  ${PROJECT_SOURCE_DIR}/src/communication/communication.c
  ${PROJECT_SOURCE_DIR}/src/cmp/cmp.c
  ${PROJECT_SOURCE_DIR}/src/wiring/arduinogalileo.c
  ${PROJECT_SOURCE_DIR}/src/wiring/raspberrypi.c
  ${PROJECT_SOURCE_DIR}/src/wiring/server.c
  ${PROJECT_SOURCE_DIR}/src/wiring/i2c.c
  ${PROJECT_SOURCE_DIR}/src/wiring/math.c
  ${PROJECT_SOURCE_DIR}/src/wiring/beaglebone.c
  ${PROJECT_SOURCE_DIR}/src/wiring/beagleboneConfig.c
  ${PROJECT_SOURCE_DIR}/src/wiring/red-pitaya.c
  ${PROJECT_SOURCE_DIR}/src/wiring/udooneo.c

  # Auxiliary
  ${PROJECT_SOURCE_DIR}/src/libraries/lcd.c
  ${PROJECT_SOURCE_DIR}/src/libraries/Print.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/rgb_lcd.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Servo.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Stepper.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Stream.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Wire.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/WString.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/SPI.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Adafruit_GFX.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Adafruit_PCD8544.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Adafruit_ST7735.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Adafruit_LEDBackpack.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Adafruit_Sensor.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Adafruit_BMP085_U.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Adafruit_MCP23008.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/SparkFun_APDS9960.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/LiquidCrystal.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/HTU21D.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/TM1637.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/SeeedOLED.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/I2Cdev.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/MPU6050.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/LED_Bar.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/CircularLED.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/UniPi.c
)

add_subdirectory (src)
add_subdirectory (languages)

if (SERVER)
else ()
  install (FILES scripts/install_social      DESTINATION bin/ PERMISSIONS WORLD_READ WORLD_EXECUTE)
  install (FILES scripts/update_libwyliodrin DESTINATION bin/ PERMISSIONS WORLD_READ WORLD_EXECUTE)
  install (FILES scripts/update_streams      DESTINATION bin/ PERMISSIONS WORLD_READ WORLD_EXECUTE)
  install (FILES scripts/say                 DESTINATION bin/ PERMISSIONS WORLD_READ WORLD_EXECUTE)
endif ()
