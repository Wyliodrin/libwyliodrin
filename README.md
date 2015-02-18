libwyliodrin - Arduino-like library for Communication on GNU/Linux platforms
============



Supported Boards
============
  * [Intel Galileo](http://arduino.cc/en/ArduinoCertified/IntelGalileo)
  * [Intel Edison](http://www.intel.com/content/www/us/en/do-it-yourself/edison.html)
  * [Raspberry Pi](http://www.raspberrypi.org/)
  * [Beaglebone Black](http://beagleboard.org/BLACK)



Requirements
============
  * [redis-server](http://redis.io/)
  * [libhiredis](https://github.com/redis/hiredis)
  * [libjansson](https://github.com/akheron/jansson)
  * [SWIG 3.0.1+](http://www.swig.org/)
  * [python](https://www.python.org/)
  * [nodejs](http://nodejs.org/)
  * [libmraa](https://github.com/intel-iot-devkit/mraa) *(for Intel Galileo only)*
  * [WiringPi](http://wiringpi.com/) *(for Raspberry Pi only)*



Install requirements
==========
~~~~~~~~~~~~~{.sh}
sudo apt-get update
sudo apt-get install -y redis-server libhiredis-dev libjansson-dev cmake swig3.0 python-dev

# Manually install nodejs
wget http://nodejs.org/dist/v0.10.28/node-v0.10.28.tar.gz
tar xfz node-v0.10.28.tar.gz
cd node-v0.10.28.tar.gz
./configure
make
sudo make install
~~~~~~~~~~~~~

Check [this](https://github.com/intel-iot-devkit/mraa/blob/master/docs/building.md) for **libmraa** installation.

Check [this](http://wiringpi.com/download-and-install/) for **WiringPi** installation.



Install
==========
~~~~~~~~~~~~~{.sh}
mkdir build
cd build
cmake -DBOARD=ON .. # BOARD can be GALILEO, RASPBERRYPI or BEAGLEBONE
make
sudo make install
~~~~~~~~~~~~~
