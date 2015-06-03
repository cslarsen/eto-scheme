CXXFLAGS := -W -Wall -g -Iinclude -std=c++14
LDFLAGS := -lgmpxx -lgmp
TARGETS := eto.o test

check: all
	./test

eto.o: eto.cpp include/eto/eto.hpp

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
