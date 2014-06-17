.PHONY: clean build install

wyliodrin_wrap.cxx:
	swig -python -c++ wyliodrin.i

build:wyliodrin_wrap.cxx
	python setup.py build

install:build
	python setup.py install

clean:
	python setup.py clean
	rm -rf build
	rm -f *.cxx
	rm -f wyliodrin.py
