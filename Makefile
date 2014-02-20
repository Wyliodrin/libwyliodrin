.PHONY: clean wyliodrin python

SRC = ${wildcard *.c}
OBJS = ${SRC:.c=.o}

wyliodrin:$(BOARD)
	echo "wyliodrin setup"
	make --directory=$(BOARD) install
	make build
	cp wyliodrin.so /usr/lib

build:$(OBJS)
	echo $(OBJS)
	gcc -shared $(OBJS) -o libwyliodrin.so
	cp Wyliodrin.h /usr/include
	cp libwyliodrin.so /usr/lib
	ldconfig	

python:
	make --directory=$(BOARD) python	
	cd pyhton/wyliodrin; python setup.py install; cd ../..

clean:
	make --directory=raspberrypi clean	
	make --directory=arduinogalileo clean
	
