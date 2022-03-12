CC = gcc
CFLAGS = -Wall -g
DIR := ${CURDIR}

all: static shared

static:
	$(CC) $(CFLAGS) -c lib_counting.c -o lib_counting.o
	ar rcs liblib_counting.a lib_counting.o

shared:
	$(CC) $(CFLAGS) -fPIC -c lib_counting.c -o lib_counting.o
	$(CC) $(CFLAGS) -shared -o liblib_counting.so lib_counting.o

clean:
	@rm -rf *.o *.a *.so file*.txt main

tests:
	rm -f raport2.txt

	make -s test_static

	make -s test_shared


test_static: static
	$(CC) -c main.c
	$(CC) main.o liblib_counting.a -o main

	@echo "~~~~~~~~~~~~~~~~~~~STATIC TEST~~~~~~~~~~~~~~~~~~~" >> raport2.txt
	make  small >> raport2.txt
	#make --no-print-directory medium >> raport2.txt
	#make --no-print-directory large >> raport2.txt


test_shared: shared
	export LD_LIBRARY_PATH=$(DIR):$LD_LIBRARY_PATH
	$(CC) main.c -llib_counting -L ./ -o main

	@echo "~~~~~~~~~~~~~~~~~~~SHARED TEST~~~~~~~~~~~~~~~~~~~" >> raport2.txt
	# --no-print-directory - to not print  “Entering/Leaving directory ...” messages
	make small >> raport2.txt
	#make --no-print-directory medium >> raport2.txt
	#make --no-print-directory large >> raport2.txt


small:
	@echo "\n########### SMALL TESTS ###########\n" >> raport2.txt
	@echo "5 files, 2 lines\n" >> raport2.txt
	@# @ makes silent
	@./main count_files: small.txt

