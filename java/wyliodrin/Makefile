build:
	swig -java wyliodrin.i
	gcc -fPIC -I/usr/lib/jvm/java-7-openjdk-i386/include -c WyliodrinCopy/Wyliodrin.c wyliodrin_wrap.c
	gcc -shared Wyliodrin.o wyliodrin_wrap.o -o libwyliodrin.so
	cp WyliodrinCopy/Wyliodrin.h /usr/include
	cp libwyliodrin.so /usr/lib
	ldconfig

test:
	javac main.java
	java main

clean:
	rm -f libwyliodrin.so
	rm -f main.class
	rm -f wyliodrin.class
	rm -f wyliodrin.java
	rm -f Wyliodrin.o
	rm -f wyliodrinJNI.class
	rm -f wyliodrinJNI.java
	rm -f wyliodrin_wrap.c
	rm -f wyliodrin_wrap.o
