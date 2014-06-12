.PHONY: clean build 

CPPFLAGS = -I . -fPIC -D$(BOARD)

SRCS = ${wildcard *.c}
OBJS = ${SRCS:.c=.o}

SRCWIRING = ${wildcard wiring/*.c}
OBJWIRING = ${SRCWIRING:.c=.o}

SRCLIBRARIES = ${wildcard libraries/*.c}
OBJLIBRARIES = ${SRCLIBRARIES:.c=.o}

SRCSIGNALS = ${wildcard signals/*.c}
OBJSIGNALS = ${SRCSIGNALS:.c=.o}

build:$(OBJS) $(OBJWIRING) $(OBJLIBRARIES) $(OBJSIGNALS)
	g++ -shared -o libwyliodrin.so $(OBJS) $(OBJWIRING) $(OBJLIBRARIES) $(OBJSIGNALS) -lpthread -lrt -lmaa -lhiredis -ljansson
	mkdir ../build
	mkdir ../build/include
	mkdir ../build/include/Wyliodrin
	cp *.h ../build/include
	cp wiring/*.h ../build/include/Wyliodrin
	cp signals/*.h ../build/include/Wyliodrin
	cp libraries/*.h ../build/include/Wyliodrin
	mkdir ../build/lib
	cp libwyliodrin.so ../build/lib

clean:
	rm -f *.o
	rm -f wiring/*.o
	rm -f libraries/*.o
	rm -f libwyliodrin.so
	rm -rf ../build

