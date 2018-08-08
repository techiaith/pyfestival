# Run with:
#    make install PYTHON=python3
# for a Python3 install
PYTHON=python

all: _festival.so

_festival.so: _festival.cpp
	CFLAGS="-g -fopenmp" $(PYTHON) setup.py build
	ln -sf build/*/_festival*.so _festival.so

install: all
	$(PYTHON) setup.py install

test: all
	$(PYTHON) test.py

clean:
	$(PYTHON) setup.py clean
	rm -rf build _festival.so *.pyc
