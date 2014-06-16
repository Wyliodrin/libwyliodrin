.PHONY: clean build 

CPPFLAGS = -I . -g -fPIC -D$(BOARD)

SRCS = ${wildcard *.c}
OBJS = ${SRCS:.c=.o}

SRCWIRING = ${wildcard wiring/*.c}
OBJWIRING = ${SRCWIRING:.c=.o}

SRCLIBRARIES = ${wildcard libraries/*.c}
OBJLIBRARIES = ${SRCLIBRARIES:.c=.o}

SRCCPPLIBRARIES = ${wildcard libraries/*.cpp}
OBJCPPLIBRARIES = ${SRCCPPLIBRARIES:.cpp=.o}

SRCSIGNALS = ${wildcard signals/*.c}
OBJSIGNALS = ${SRCSIGNALS:.c=.o}

build:$(OBJS) $(OBJWIRING) $(OBJLIBRARIES) $(OBJCPPLIBRARIES) $(OBJSIGNALS)
	g++ -shared -o libwyliodrin.so $(OBJS) $(OBJWIRING) $(OBJLIBRARIES) $(OBJCPPLIBRARIES) $(OBJSIGNALS) -lpthread -lrt -lmaa -lhiredis -ljansson
	mkdir -p ../build
	mkdir -p ../build/include
	mkdir -p ../build/include/Wyliodrin
	cp *.h ../build/include
	cp wiring/*.h ../build/include/Wyliodrin
	cp signals/*.h ../build/include/Wyliodrin
	cp libraries/*.h ../build/include/Wyliodrin
	mkdir -p ../build/lib
	cp libwyliodrin.so ../build/lib

clean:
	rm -f *.o
	rm -f wiring/*.o
	rm -f signals/*.o
	rm -f libraries/*.o
	rm -f libwyliodrin.so
	rm -rf ../build

