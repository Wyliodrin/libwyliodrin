.PHONY: clean wyliodrin python

SRC = ${wildcard *.c}

wyliodrin:$(BOARD)
	echo "wyliodrin setup"
	make --directory=$(BOARD) install
	make build
	cp wyliodrin.so /usr/lib

build:objs
	gcc -shared *.o -o libwyliodrin.so
	cp Wyliodrin.h /usr/include
	cp libwyliodrin.so /usr/lib
	ldconfig

objs:$(SRC)
	gcc -fPIC -c $(SRC)

python:
	make --directory=$(BOARD) python	
	cd pyhton/wyliodrin; python setup.py install; cd ../..

clean:
	make --directory=raspberrypi clean	
	make --directory=arduinogalileo clean
	
