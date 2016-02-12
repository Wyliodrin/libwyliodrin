add_library (wyliodrin SHARED ${wyliodrin_LIB_SRCS})
target_link_libraries (wyliodrin ${HIREDIS_LIBRARIES} ${JANSSON_LIBRARIES} ${MRAA_LIBRARIES} ${WIRINGPI_LIBRARIES} ${REDPITAYA_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} -lrt)

set_target_properties (
  wyliodrin
  PROPERTIES
  SOVERSION ${VERSION_MAJOR}
  VERSION ${VERSION_MINOR}
)

file (GLOB wyiliodrin_headers
  "communication/*.h"
  "signals/*.h"
  "libraries/*.h"
  "wiring/*.h"
  "cmp/cmp.h"
  "setup.h"
  "version.h"
)

install (TARGETS wyliodrin DESTINATION lib)
install (FILES Arduino.h Wyliodrin.h DESTINATION include/)
install (FILES ${wyiliodrin_headers} DESTINATION include/Wyliodrin/)
