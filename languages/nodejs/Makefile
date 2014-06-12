.PHONY: clean build install

wyliodrin_wrap.cxx:
	swig -javascript -node -c++ wyliodrin.i

build:wyliodrin_wrap.cxx

install:build
	npm install -g

clean:
	rm -rf build
	rm -f *.cxx