add_executable (libwyliodrin_version libwyliodrin_version.c)

target_link_libraries (libwyliodrin_version LINK_PUBLIC wyliodrin event)

install (TARGETS libwyliodrin_version DESTINATION bin)
