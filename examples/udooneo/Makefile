all: gpio
.PHONY: all

gpio: gpio.c
	gcc -Wall gpio.c -o gpio -lwyliodrin -levent
.PHONY: blink

clean:
	rm -r gpio
.PHONY: clean
