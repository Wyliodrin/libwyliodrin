.PHONY: clean build install

wyliodrin_wrap.c:
	swig -python wyliodrin.i

build:wyliodrin_wrap.c
	python setup.py build

install:build
	python setup.py install

clean:
	python setup.py clean
	rm -rf build
	rm -f *.c
	rm -f wyliodrin.py
