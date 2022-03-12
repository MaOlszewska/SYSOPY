CC = gcc -Wall -std=c11 -ggdb

all: static shared clean

static: lib_counting.o
		ar rcs lib_counting.a lib_counting.o

shared: lib_counting.o
		$(CC) -c -fPIC lib_counting.c
		$(CC) -shared -fPIC -o lib_counting.so lib_counting.o

lib_counting.o: lib_counting.c lib_counting.h
		$(CC) -c lib_counting.c

clean:
		rm -f *.o

clean-library:
		rm -f *.so
		rm -f *.a