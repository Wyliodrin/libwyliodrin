FIND_PACKAGE (PythonLibs)                                                                                  
INCLUDE_DIRECTORIES (${PYTHON_INCLUDE_PATH})

include_directories (
  ${CMAKE_SOURCE_DIR}/src/
  ${PYTHON_INCLUDE_DIRS}
)

set_source_files_properties (wyliodrin.i PROPERTIES CPLUSPLUS ON)
set(SWIG_FLAGS "-I${CMAKE_SOURCE_DIR}/src" "${FLAGS}")
set_source_files_properties (wyliodrin.i PROPERTIES SWIG_FLAGS "${SWIG_FLAGS}")
swig_add_module (wyliodrin python wyliodrin.i ${wyliodrin_LIB_SRCS})
#swig_link_libraries (wyliodrin ${PYTHON_LIBRARIES} ${HIREDIS_LIBRARIES} ${JANSSON_LIBRARIES} ${MRAA_LIBRARIES} ${CMAKE_CURRENT_BINARY_DIR}/../../src/libwyliodrin.so ${CMAKE_THREAD_LIBS_INIT} -lrt)
#add_dependencies (${SWIG_MODULE_wyliodrin_REAL_NAME} wyliodrin)
swig_link_libraries (wyliodrin ${PYTHON_LIBRARIES} ${HIREDIS_LIBRARIES} ${JANSSON_LIBRARIES} ${WIRINGPI_LIBRARIES} ${MRAA_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} -lrt)

# Essentially do seperate_arguments but with "." instead of " "
string (REPLACE "." ";" PYTHON_VERSION_LIST ${PYTHONLIBS_VERSION_STRING})
list (GET PYTHON_VERSION_LIST 0 PYTHON_VERSION_MAJOR)
list (GET PYTHON_VERSION_LIST 1 PYTHON_VERSION_MINOR)

if (GALILEO or EDISON)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/_wyliodrin.so
         ${CMAKE_CURRENT_BINARY_DIR}/wyliodrin.py
         DESTINATION lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages/)
elseif (RASPBERRYPI)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/_wyliodrin.so
         ${CMAKE_CURRENT_BINARY_DIR}/wyliodrin.py
         DESTINATION lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/dist-packages/)
else ()
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/_wyliodrin.so
         ${CMAKE_CURRENT_BINARY_DIR}/wyliodrin.py
         DESTINATION lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages/)
endif ()
