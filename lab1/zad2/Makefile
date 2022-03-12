CC = gcc
CFLAGS = -Wall -g
DIR := ${CURDIR}

clean:
	@rm -rf *.o *.a *.so file*.txt main

test:
	make -C ../zad1/ static
	$(CC) -c main.c
	$(CC) main.o ../zad1/lib_counting.a -o main

	@echo "SMALL TESTS\n\n\n" >> raport2.txt

	./main count_files: small2.txt small3.txt count_files: small4.txt count_files: small1.txt small2.txt small3.txt small4.txt >> raport2.txt
	@echo "\n------------------------\n" >> raport2.txt
	./main count_files: small2.txt small3.txt count_files: small4.txt count_files: small1.txt small2.txt small3.txt small4.txt  remove: 0 remove: 1 >> raport2.txt
	@echo "\n------------------------\n" >> raport2.txt
	./main count_files: small.txt remove: 0 count_files: small1.txt remove: 1 >> raport2.txt

	@echo "\n\n\nMEDIUM TESTS \n\n\n" >> raport2.txt
	./main count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt >> raport2.txt
	@echo "\n------------------------\n" >> raport2.txt
	./main count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt remove: 0 remove: 1 >> raport2.txt
	@echo "\n------------------------\n" >> raport2.txt
	./main count_files: medium1.txt medium2.txt medium3.txt medium1.txt medium2.txt medium3.txt remove: 0 count_files: medium1.txt medium2.txt medium3.txt medium1.txt medium2.txt medium1.txt medium3.txt remove: 1 >> raport2.txt
	@echo "\n------------------------\n" >> raport2.txt
	./main count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt >> raport2.txt

	@echo "\n\n\nLARGE TESTS \n\n\n" >> raport2.txt
	./main count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt count_files: large1.txt medium2.txt large3.txt >> raport2.txt
	@echo "\n------------------------\n" >> raport2.txt
	./main count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt remove: 0 remove: 1 >> raport2.txt
	@echo "\n------------------------\n" >> raport2.txt
	./main count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt remove: 0 count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt remove: 1 >> raport2.txt
	@echo "\n------------------------\n" >> raport2.txt
	./main count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large1.txt large2.txt large3.txt remove: 1 >> raport2.txt