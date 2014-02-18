.PHONY: clean install

install:
	make --directory=../../arduino clean
	make --directory=../../arduino LIBRARY=wiring
	cp ../../arduino/libwiringarduino.so .
	python setup.py install
	# gcc -c -fPIC arduino_wrap.c -I /usr/include/python2.7
	# g++ -shared arduino_wrap.o ../../arduino/*.o -o _arduino.so -lpthread -lrt