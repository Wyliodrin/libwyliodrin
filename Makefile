.PHONY: clean wyliodrin python

SRC = ${wildcard *.c}
OBJS = ${$SRC:.cpp=.o}

wyliodrin:$(BOARD) $(OBJS)
	echo "wyliodrin setup"
	make --directory=$(BOARD) install
	gcc -shared $(OBJS) -o wyliodrin.so
	cp wyliodrin.so /usr/lib

python:
	make --directory=$(BOARD) python	

clean:
	make --directory=raspberrypi clean	
	make --directory=arduinogalileo clean
	
