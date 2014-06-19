cmake_minimum_required (VERSION 2.8)                                                                                                                           
project (libwyliodrin)

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# Appends the cmake/modules path to MAKE_MODULE_PATH variable.                                                                                                 
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

find_package (Threads)

find_package (SWIG)
if (SWIG_FOUND)                                                                                                                                                
  include (${SWIG_USE_FILE})
endif ()

find_package (PkgConfig REQUIRED)
pkg_check_modules (MAA REQUIRED maa>=0.2.6)
message (INFO " found libmaa version: ${MAA_VERSION}")

find_package(Hiredis REQUIRED)
include_directories(${HIREDIS_INCLUDE_DIR})

find_package(Jansson REQUIRED)
include_directories(${JANSSON_INCLUDE_DIR})

set (wyliodrin_VERSION_MAJOR 1)
set (wyliodrin_VERSION_MINOR 2)
set (wyliodrin_VERSION_PATCH 0})

option (GALILEO "Build for the Intel Galileo." OFF)
option (RASPBERRYPI "Build for the RaspberryPi." OFF)
if (GALILEO)
  add_definitions(-DARDUINOGALILEO)
elseif (RASPBERRYPI)
  add_definitions(-DRASPBERRYPI)
else ()
  message (FATAL_ERROR "You need to choose between -DRASPBERRYPI=ON or -DGALILEO=ON")
endif ()

set (wyliodrin_LIB_SRCS                                                                                   
  ${PROJECT_SOURCE_DIR}/src/Wyliodrin.c
  ${PROJECT_SOURCE_DIR}/src/signals/signals.c
  ${PROJECT_SOURCE_DIR}/src/libraries/lcd.c
  ${PROJECT_SOURCE_DIR}/src/libraries/Print.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/rgb_lcd.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Servo.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Stepper.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Stream.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Wire.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/WString.cpp
  ${PROJECT_SOURCE_DIR}/src/wiring/arduinogalileo.c
  ${PROJECT_SOURCE_DIR}/src/wiring/math.c
)

add_subdirectory (src)
add_subdirectory (languages)
