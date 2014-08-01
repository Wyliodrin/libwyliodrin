all: blinkUserLed bouncingBall policeSiren pulseRateMonitor

blinkUserLed: blinkUserLed.c
	gcc -Wall blinkUserLed.c -o blinkUserLed -lwyliodrin
.PHONY: blinkUserLed

bouncingBall: bouncingBall.c
	gcc -Wall bouncingBall.c -o bouncingBall -lwyliodrin
.PHONY: bouncingBall

policeSiren: policeSiren.c
	gcc -Wall policeSiren.c -o policeSiren -lwyliodrin
.PHONY: policeSiren

pulseRateMonitor: pulseRateMonitor.c
	gcc -Wall pulseRateMonitor.c -o pulseRateMonitor -lwyliodrin
.PHONY: pulseRateMonitor

clean:
	rm blinkUserLed bouncingBall policeSiren pulseRateMonitor
.PHONY: clean