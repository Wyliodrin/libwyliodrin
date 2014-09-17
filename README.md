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
    cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr -DBOARD=ON -DFIRMATA .. - BOARD can be GALILEO, RASPBERRYPI or UDOO
        -DFIRMATA works for UDOO only
    make
    sudo make install
  

      

