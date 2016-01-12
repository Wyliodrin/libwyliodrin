libwyliodrin
============

TODO - description here



Requirements
------------
Libwyliodrin has the following requirements
  * redis-server
  * libhiredis
  * libjansson

Raspberry Pi
  * wiringPi

ArduinoGalileo
  * libmraa

Languages
  * swig 3.0.1+

Python
  * python
  * distools

Javascript
  * node
  * npm



Install
-------
```
  mkdir build
  cd build
  cmake -DBOARD=ON .. // BOARD can be RASPBERRYPI, GALILEO, EDISON, BEAGLEBONE, REDPITAYA, UDOONEO or SERVER
  make
  sudo make install
```



Files
-----
```
/usr/bin/install_social
/usr/bin/update_libwyliodrin
/usr/bin/update_streams
/usr/bin/say

/usr/lib/libwyliodrin.so.0
/usr/lib/libwyliodrin.so.3
/usr/lib/libwyliodrin.so

/usr/include/Arduino.h
/usr/include/Wyliodrin.h
/usr/include/Wyliodrin/*

/usr/lib/python2.7/dist-packages/_wyliodrin.so
/usr/lib/python2.7/dist-packages/wyliodrin.py

/usr/lib/node_modules/wyliodrin/package.json
/usr/lib/node_modules/wyliodrin/wyliodrin.node
```
