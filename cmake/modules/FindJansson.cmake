find_path(JANSSON_INCLUDE_DIR
NAMES
jansson.h
PATHS
/usr/include
/usr/local/include
)

find_library(JANSSON_LIBRARY
NAMES
jansson
PATHS
/usr/lib
/usr/local/lib
)
