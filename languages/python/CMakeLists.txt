FIND_PACKAGE (PythonLibs)                                                                                  
INCLUDE_DIRECTORIES (${PYTHON_INCLUDE_PATH})

include_directories (
  ${CMAKE_SOURCE_DIR}/src/
  ${PYTHON_INCLUDE_DIRS}
)

set_source_files_properties (wyliodrin.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties (wyliodrin.i PROPERTIES SWIG_FLAGS "-I${CMAKE_BINARY_DIR}/src")
swig_add_module (wyliodrin python wyliodrin.i)
swig_link_libraries (wyliodrin ${PYTHON_LIBRARIES})

# Essentially do seperate_arguments but with "." instead of " "
string (REPLACE "." ";" PYTHON_VERSION_LIST ${PYTHONLIBS_VERSION_STRING})
list (GET PYTHON_VERSION_LIST 0 PYTHON_VERSION_MAJOR)
list (GET PYTHON_VERSION_LIST 1 PYTHON_VERSION_MINOR)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/_wyliodrin.so
         ${CMAKE_CURRENT_BINARY_DIR}/wyliodrin.py
         DESTINATION lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages/)
