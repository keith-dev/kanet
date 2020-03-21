.POSIX:
#.SUFFIX:

PREFIX = /usr/local
CXX = c++
CXXFLAGS = -g -std=c++17 -pedantic -Wall -Wextra -DSPDLOG_FMT_EXTERNAL -I$(PREFIX)/include
LDFLAGS = -L$(PREFIX)/lib -lspdlog -lfmt

PROG = test

.PHONY: all clean clean-all
all: $(PROG)

clean-all: clean
	- rm main.o

clean:
	- rm $(PROG)

$(PROG): main.o
	$(CXX) -o $^ $@ $(LDFLAGS)

main.o: main.cpp address.hpp
