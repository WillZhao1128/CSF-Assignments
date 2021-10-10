CC=g++
CFLAGS=-Wall -Wextra -std=c++11 -pedantic -g

EXES = csim
# Links together files needed to create executable

%.o : %.c
	$(CC) $(CFLAGS) -c $*.c -o $*.o

all : $(EXES)

# targets for executables
csim: main.o
	$(CC) -o $@ main.o

# targets for object files
main.o: main.cpp

clean:
	rm -f *.o csim