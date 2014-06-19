set (wyliodrin_LIB_SRCS
  ${PROJECT_SOURCE_DIR}/src/Wyliodrin.c
  ${PROJECT_SOURCE_DIR}/src/signals/signals.c
  ${PROJECT_SOURCE_DIR}/src/libraries/lcd.c
  ${PROJECT_SOURCE_DIR}/src/libraries/Print.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/rgb_lcd.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Servo.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Stepper.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Stream.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/Wire.cpp
  ${PROJECT_SOURCE_DIR}/src/libraries/WString.cpp
  ${PROJECT_SOURCE_DIR}/src/wiring/arduinogalileo.c
  ${PROJECT_SOURCE_DIR}/src/wiring/math.c
)

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
install (FILES Arduino.h Wyliodrin.h DESTINATION include/)
install (FILES ${wyiliodrin_headers} DESTINATION include/Wyliodrin/)
