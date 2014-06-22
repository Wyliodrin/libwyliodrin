add_library (wyliodrin SHARED ${wyliodrin_LIB_SRCS})
target_link_libraries (wyliodrin ${HIREDIS_LIBRARIES} ${JANSSON_LIBRARIES} ${MAA_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} -lrt)

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
)

install (TARGETS wyliodrin DESTINATION lib)
install (FILES scripts/install_social.sh DESTINATION bin/)
install (FILES Arduino.h Wyliodrin.h DESTINATION include/)
install (FILES ${wyiliodrin_headers} DESTINATION include/Wyliodrin/)
