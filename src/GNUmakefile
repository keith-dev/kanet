PROG_CXX  = srv
SRCS      = main.cpp

CXXFLAGS += -g -std=c++17 -pedantic -Wall -Wextra -DSPDLOG_FMT_EXTERNAL -I../include -I/usr/local/include
LDFLAGS  += -L/usr/local/lib -lspdlog -lfmt

all: $(PROG_CXX)

clean:
	-rm $(PROG_CXX) $(SRCS:.cpp=.o)

$(PROG_CXX): $(SRCS:.cpp=.o)
	$(LINK.cc) $^ -o $@ $(LDFLAGS)
