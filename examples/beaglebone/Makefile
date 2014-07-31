all: blinkUserLed bouncingBall

blinkUserLed: blinkUserLed.c
	gcc -Wall blinkUserLed.c -o blinkUserLed -lwyliodrin
.PHONY: blinkUserLed

bouncingBall: bouncingBall.c
	gcc -Wall bouncingBall.c -o bouncingBall -lwyliodrin

clean:
	rm blinkUserLed bouncingBall
.PHONY: clean