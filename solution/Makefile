CC=g++
CFLAGS=-g -Wall -Wextra -std=c++11 -pedantic

EXES = csim
# Links together files needed to create executable

%.o : %.cpp
	$(CC) $(CFLAGS) -c $*.cpp -o $*.o

all : $(EXES)

# targets for executables
csim: main.o helper.o cache.o set.o
	$(CC) $(CFLAGS) -o $@ main.o helper.o cache.o set.o

# targets for object files
main.o: main.cpp

helper.o: helper.cpp helper.h

cache.o: cache.h cache.cpp

set.o: set.h set.cpp block.h

clean:
	rm -f *.o csim