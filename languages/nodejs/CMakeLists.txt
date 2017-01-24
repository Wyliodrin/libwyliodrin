
find_path (NODE_ROOT_DIR NAMES "node/node.h" "nodejs/node.h" "nodejs/src/node.h")

set (NODEJS_INCLUDE_DIRS
  ${NODE_ROOT_DIR}/node
  ${NODE_ROOT_DIR}/nodejs
  ${NODE_ROOT_DIR}/nodejs/src
)

find_package (Nodejs REQUIRED)

include_directories (
  ${NODEJS_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/src
)

message (STATUS "INFO - swig Version ${SWIG_VERSION}")

set (V8_VERSION_HEX 0x0${V8_VERSION_MAJOR}${V8_VERSION_MINOR}${V8_VERSION_PATCH})
string (LENGTH "${V8_VERSION_HEX}" V8_VERSION_HEX_length)
while (V8_VERSION_HEX_length LESS 8)
  set (V8_VERSION_HEX "${V8_VERSION_HEX}0")
  message (STATUS "INFO - Padded V8 version to match SWIG format")
  string (LENGTH "${V8_VERSION_HEX}" V8_VERSION_HEX_length)
endwhile ()


set (CMAKE_CXX_FLAGS -DBUILDING_NODE_EXTENSION)
set (CMAKE_CXX_FLAGS "-std=gnu++11")

set_property (SOURCE wyliodrin.i PROPERTY SWIG_FLAGS "-node"
              "-I${CMAKE_BINARY_DIR}/src" "-DV8_VERSION=${V8_VERSION_HEX}")

#set_source_files_properties (wyliodrin.i PROPERTIES SWIG_FLAGS "-node;-I${CMAKE_BINARY_DIR}/src")
set_source_files_properties (wyliodrin.i PROPERTIES CPLUSPLUS ON)
swig_add_module (wyliodrinjs javascript wyliodrin.i  ${wyliodrin_LIB_SRCS})
swig_link_libraries (wyliodrinjs ${NODE_LIBRARIES} ${HIREDIS_LIBRARIES} ${JANSSON_LIBRARIES} ${WIRINGPI_LIBRARIES} ${MRAA_LIBRARIES} ${REDPITAYA_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} -lrt -levent)

set_target_properties (wyliodrinjs PROPERTIES
  PREFIX ""
  OUTPUT_NAME "wyliodrin"
  SUFFIX ".node"
)

set_property (TARGET wyliodrinjs PROPERTY CXX_STANDARD 11)
set_property (TARGET wyliodrinjs PROPERTY CXX_STANDARD_REQUIRED ON)

install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/package.json
         ${CMAKE_CURRENT_BINARY_DIR}/wyliodrin.node
         DESTINATION lib/node_modules/wyliodrin)
