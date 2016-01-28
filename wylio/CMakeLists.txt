cmake_minimum_required (VERSION 2.8.8)

project (wylio)

set (CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -Wall")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# Appends the cmake/modules path to MAKE_MODULE_PATH variable.
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})



# Dependencies
find_package (PkgConfig REQUIRED)
find_package (Hiredis REQUIRED)
include_directories (${HIREDIS_INCLUDE_DIR})
find_package (Jansson REQUIRED)
include_directories (${JANSSON_INCLUDE_DIR})
pkg_check_modules (EVENT REQUIRED libevent)


add_definitions ("-Wall -DDEBUG")


include_directories (${PROJECT_BINARY_DIR})



# Sources
set (wylio_SRCS
  # Core
  ${PROJECT_SOURCE_DIR}/wylio.c
)

add_executable (wylio
  ${wylio_SRCS}
)

target_link_libraries (wylio wyliodrin -levent) 



install (TARGETS wylio DESTINATION bin)


