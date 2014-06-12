.PHONY: clean

LIBRARY:=arduino

cppflags.arduino := -DARDUINO
cppflags.wiring := 

output.arduino := arduino
output.wiring := wiringarduino

CPPFLAGS = -I . -fPIC ${cppflags.${LIBRARY}}

SRCS = ${wildcard *.c}
OBJS = ${SRCS:.c=.o}

SRCCPPS = ${wildcard *.cpp}
OBJCPPS = ${SRCCPPS:.cpp=.o}

SRCLIBRARIES = ${wildcard libraries/*.c}
OBJLIBRARIES = ${SRCLIBRARIES:.c=.o}

SRCCPPLIBRARIES = ${wildcard libraries/*.cpp}
OBJCPPLIBRARIES = ${SRCCPPLIBRARIES:.cpp=.o}


arduino:$(OBJS) $(OBJCPPS) $(OBJLIBRARIES) $(OBJCPPLIBRARIES)
	g++ -shared -o lib${output.${LIBRARY}}.so $(OBJS) $(OBJCPPS) $(OBJLIBRARIES) $(OBJCPPLIBRARIES) -lpthread -lrt 

run:arduino
	g++ arduino.so -o run

install:
	cp *.h /usr/include
	cp lib${output.${LIBRARY}}.so /usr/lib/

clean:
	rm *.o; true
	rm libraries/*.o; true
	rm libarduino.so libwiringarduino.so; true

