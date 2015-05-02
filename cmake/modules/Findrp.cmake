# FindRedPitaya.cmake - Try to find the wiringPi library

FIND_PATH(REDPITAYA_INCLUDE_DIR NAMES rp.h
   PATHS
   /usr
   /usr/local
   /opt
   PATH_SUFFIXES
   )

FIND_LIBRARY(REDPITAYA_LIBRARIES NAMES rp
   PATHS
   /usr/lib
   /usr/local/lib
   /opt/lib
   )


INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(rp DEFAULT_MSG REDPITAYA_LIBRARIES REDPITAYA_INCLUDE_DIR)

MARK_AS_ADVANCED(REDPITAYA_INCLUDE_DIR REDPITAYA_LIBRARIES)
