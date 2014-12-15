
all: festival.so

festival.so: _festival.cpp
	CFLAGS="-g" python setup.py build
	ln -sf build/*/festival*.so festival.so

clean:
	python setup.py clean
	rm -rf build festival.so *.pyc
