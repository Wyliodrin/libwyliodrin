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
    cmake -DBOARD=ON .. - BOARD can be GALILEO, RASPBERRYPI or BEAGLEBONE
    make
    sudo make install
  

      

