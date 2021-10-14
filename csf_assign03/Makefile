CC=g++
CFLAGS=-g -Wall -Wextra -std=c++11 -pedantic

EXES = csim
# Links together files needed to create executable

%.o : %.cpp
	$(CC) $(CFLAGS) -c $*.cpp -o $*.o

all : $(EXES)

# targets for executables
csim: main.o helper.o cache.o
	$(CC) $(CFLAGS) -o $@ main.o helper.o cache.o

# targets for object files
main.o: main.cpp

helper.o: helper.cpp helper.h

cache.o: cache.h cache.cpp

clean:
	rm -f *.o csim