add_executable (wylio wylio.c)

target_link_libraries (wylio LINK_PUBLIC wyliodrin event)

install (TARGETS wylio DESTINATION bin)
