find_path (NODE_ROOT_DIR "node/node.h")                                                                   

set (NODE_INCLUDE_DIRS
  ${NODE_ROOT_DIR}/src
  ${NODE_ROOT_DIR}/node
  ${NODE_ROOT_DIR}/deps/v8/include
  ${NODE_ROOT_DIR}/deps/uv/include
)

include_directories (
  ${NODE_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/src
)

set (CMAKE_CXX_FLAGS -DBUILDING_NODE_EXTENSION)

set_source_files_properties (wyliodrin.i PROPERTIES SWIG_FLAGS "-node;-I${CMAKE_BINARY_DIR}/src")
set_source_files_properties (wyliodrin.i PROPERTIES CPLUSPLUS ON)
swig_add_module (wyliodrinjs javascript wyliodrin.i)
swig_link_libraries (wyliodrinjs ${NODE_LIBRARIES})

set_target_properties (wyliodrinjs PROPERTIES
  PREFIX ""
  SUFFIX ".node"
)

install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/package.json
         ${CMAKE_CURRENT_BINARY_DIR}/wyliodrinjs.node
         DESTINATION lib/node_modules/wyliodrin)