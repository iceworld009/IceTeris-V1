CC      = gcc
CXX     = g++
CFLAGS  = -I.
CXXFLAGS= -I. -std=c++17

DEPS = conio.h
OBJS = main.o conio.o

all: main

main: $(OBJS)
	$(CXX) -o $@ $(OBJS)

# C source
conio.o: conio.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

# C++ source (note: depends on conio.h so it rebuilds when header changes)
main.o: main.cpp $(DEPS)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	rm -f main $(OBJS)
