all: blinkUserLed bouncingBall policeSiren

blinkUserLed: blinkUserLed.c
	gcc -Wall blinkUserLed.c -o blinkUserLed -lwyliodrin
.PHONY: blinkUserLed

bouncingBall: bouncingBall.c
	gcc -Wall bouncingBall.c -o bouncingBall -lwyliodrin
.PHONY: bouncingBall

policeSiren: policeSiren.c
	gcc -Wall policeSiren.c -o policeSiren -lwyliodrin
.PHONY: policeSiren

clean:
	rm blinkUserLed bouncingBall policeSiren
.PHONY: clean