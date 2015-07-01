# FindWiringpi.cmake - Try to find the wiringPi library
# Once done this will define
#
#  HIREDIS_FOUND - System has Hiredis
#  HIREDIS_INCLUDE_DIR - The Hiredis include directory
#  HIREDIS_LIBRARIES - The libraries needed to use Hiredis
#  HIREDIS_DEFINITIONS - Compiler switches required for using Hiredis

FIND_PATH(WIRINGPI_INCLUDE_DIR NAMES wiringPi.h
   PATHS
   /usr
   /usr/local
   /opt
   PATH_SUFFIXES
   )

FIND_LIBRARY(WIRINGPI_LIBRARY NAMES wiringPi
   PATHS
   /usr
   /usr/local
   /opt
   )


set(WIRINGPI_INCLUDE_DIRS
  ${WIRINGPI_INCLUDE_DIR}
  )
if (WIRINGPI_LIBRARY)
  set(WIRINGPI_LIBRARIES
    ${WIRINGPI_LIBRARIES}
    ${WIRINGPI_LIBRARY}
    )
endif (WIRINGPI_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(wiringPi DEFAULT_MSG WIRINGPI_LIBRARIES WIRINGPI_INCLUDE_DIR)

MARK_AS_ADVANCED(WIRINGPI_INCLUDE_DIR WIRINGPI_LIBRARIES)

