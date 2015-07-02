add_library (wyliodrin SHARED ${wyliodrin_LIB_SRCS})
target_link_libraries (wyliodrin ${HIREDIS_LIBRARIES} ${JANSSON_LIBRARIES} ${MRAA_LIBRARIES} ${WIRINGPI_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} -lrt)

set_target_properties (
  wyliodrin
  PROPERTIES
  SOVERSION ${wyliodrin_VERSION_MAJOR}
  VERSION ${wyliodrin_VERSION_MINOR}
)

file (GLOB wyiliodrin_headers
  "signals/*.h"
  "libraries/*.h"
  "wiring/*.h"
  "cmp/cmp.h"
)

install (TARGETS wyliodrin DESTINATION lib)
install (FILES Arduino.h Wyliodrin.h DESTINATION include/)
install (FILES ${wyiliodrin_headers} DESTINATION include/Wyliodrin/)
