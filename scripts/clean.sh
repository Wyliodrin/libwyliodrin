#!/bin/bash

if [ ! "$(whoami)" = "root" ]; then
  printf 'ERROR: This script must be run as root\n' 1>&2
  exit 1
fi

rm -rf                                                                         \
  /usr/bin/install_social                                                      \
  /usr/bin/update_libwyliodrin                                                 \
  /usr/bin/update_streams                                                      \
  /usr/bin/say                                                                 \
                                                                               \
  /usr/lib/libwyliodrin.so*                                                    \
                                                                               \
  /usr/include/Arduino.h                                                       \
  /usr/include/Wyliodrin.h                                                     \
  /usr/include/Wyliodrin                                                       \
                                                                               \
  /usr/lib/python2.7/dist-packages/_wyliodrin.so                               \
  /usr/lib/python2.7/dist-packages/wyliodrin.py                                \
  /usr/lib/python2.7/site-packages/_wyliodrin.so                               \
  /usr/lib/python2.7/site-packages/wyliodrin.py                                \
                                                                               \
  /usr/lib/node_modules/wyliodrin/package.json                                 \
  /usr/lib/node_modules/wyliodrin/wyliodrin.node                            && \
                                                                               \
echo "Successfully removed libwyliodrin and all its files" && exit 0

echo "Could not remove libwyliodrin and all its files"
exit 1
