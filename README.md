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
    
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr -DBOARD=ON .. - BOARD can be GALILEO or RASPBERRYPI or BEAGLEBONE
    make
    sudo make install
  

      

