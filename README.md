libwyliodrin
============


Requirements
------------
Libwyliodrin has the following requirements
  * redis-server
  * libhiredis
  * libjansson

Raspberry Pi
  * wiringPi

ArduinoGalileo
  * libmaa

Languages
  * swig

Python
  * python
  * distools

Javascript
  * node
  * npm


Install
-------
    
    mkdir build
    cd build
    cmake -DBOARD=ON .. - BOARD can be GALILEO or RASPBERRYPI
    make
    sudo make install
  

      

