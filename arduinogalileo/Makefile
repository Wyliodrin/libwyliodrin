.PHONY: install python clean

install:
	# arduino
	make --directory=arduino LIBRARY=arduino
	make --directory=arduino install
	make --directory=arduino clean
	# wiring
	make --directory=arduino LIBRARY=wiring
	make --directory=arduino install
	# bass
	cp bass/lib* /usr/local/lib; cp bass/libbass.so /usr/lib; ldconfig

clean:
	make --directory=arduino clean
