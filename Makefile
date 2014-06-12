.PHONY: clean raspberrypi arduinogalileo install

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

# wyliodrin:$(BOARD)
# 	echo "wyliodrin setup"
# 	make --directory=$(BOARD) install
# 	make build
# 	cp wyliodrin.so /usr/lib

# build:objs
# 	gcc -shared *.o -o libwyliodrin.so
# 	cp Wyliodrin.h /usr/include
# 	cp libwyliodrin.so /usr/lib
# 	ldconfig

#objs:$(SRC)
#	gcc -fPIC -c $(SRC)

#python:
#	make --directory=$(BOARD) python	
#	cd pyhton/wyliodrin; python setup.py install; cd ../..

install:
	test $PREFIX || PREFIX=/usr/
	cp -r build/* $$PREFIX

clean:
	cd src && make clean
#	make --directory=arduinogalileo clean
	
