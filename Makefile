.PHONY: clean raspberrypi arduinogalileo install python javascript

ifeq ($(PREFIX),)
PREFIX=/usr/
endif

SRC = ${src/wildcard *.c}

all:
	@echo "Please choose a target"
	@echo "targets:"
	@echo "  raspberrypi"
	@echo "  arduinogalileo"

raspberrypi:
	echo "Building for Raspberry Pi"
	cd src && BOARD=RASPBERRYPI make

arduinogalileo:
	echo "Building for Arduino Galileo"
	cd src && BOARD=ARDUINOGALILEO make

python:
	cd languages/python && make install

javascript:
	cd languages/nodejs && make install

install:
	echo $(PREFIX)
	cp -r build/* $(PREFIX)

clean:
	cd src && make clean
	cd languages/python && make clean
	cd languages/nodejs && make clean

	
