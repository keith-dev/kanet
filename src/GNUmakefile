PROG_CXX  = srv
SRCS      = main.cpp

CXXFLAGS += -g -std=c++17 -pedantic -Wall -Wextra -DSPDLOG_FMT_EXTERNAL -I../include -I/usr/local/include
LDFLAGS  += -L/usr/local/lib -lspdlog -lfmt

DEPENDS  = $(PROG_CXX).depends

.PHONY: clean

all: $(DEPENDS) $(PROG_CXX)

clean:
	-rm $(DEPENDS) $(PROG_CXX) $(SRCS:.cpp=.depend) $(SRCS:.cpp=.o)

$(PROG_CXX): $(PROG_CXX).debug
	objcopy --strip-debug --add-gnu-debuglink=$< $@.full $@

$(PROG_CXX).debug: $(PROG_CXX).full
	objcopy --only-keep-debug $< $@

$(PROG_CXX).full: $(SRCS:.cpp=.o)
	$(LINK.cc) $^ -o $@ $(LDFLAGS)

$(DEPENDS): $(SRCS:.cpp=.depend)
	cat $(SRCS:.cpp=.depend) > $(DEPENDS)

%.depend: %.cpp
	$(COMPILE.cpp) -o $@ -MM $<

-include $(DEPENDS)
