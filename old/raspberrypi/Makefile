.PHONY: install python clean

install:
	# wiringPi
	cd wiringPi; ./build
	# arduino
	cp arduino/Arduino.h /usr/local/include
	# bass
	cp bass/lib* /usr/local/lib; ldconfig

python:
	cd python/WiringPi2-Python; ./build.sh
	cd python/pybass; python setup.py install

clean:
	cd raspberrypi/wiringPi; ./build clean 
 
