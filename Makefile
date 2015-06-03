CXXFLAGS := -W -Wall -g -Iinclude -std=c++14
LDFLAGS := -lgmpxx -lgmp
TARGETS := eto.o test
LINK.o := $(LINK.cc)

check: all
	./test

eto.o: eto.cpp include/eto/eto.hpp
test: test.o eto.o

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
