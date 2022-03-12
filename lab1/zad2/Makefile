CC = gcc
CFLAGS = -Wall -g
DIR := ${CURDIR}

clean:
	@rm -rf *.o *.a *.so file*.txt main


test:
	make -C ../zad1/ static
	$(CC) -c main.c
	$(CC) main.o ../zad1/lib_counting.a -o main

	@echo "~~~~~~~~~~~~~~~~~~~STATIC TEST~~~~~~~~~~~~~~~~~~~" > raport2.txt


	@echo "\n########### SMALL TESTS ###########\n" >> raport2.txt
	@echo "5 files, 2 lines\n" >> raport2.txt
	./main count_files: small2.txt small3.txt remove: 0 count_files: small4.txt >> raport2.txt

