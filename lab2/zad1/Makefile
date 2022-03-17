CC = gcc -Wall -g

all: test clean

test:
	$(CC) main.c -o main

	@echo "SMALL TESTS: \n" >> raport1.txt

	./main firstStest.txt secondStest.txt >> raport1.txt

	@echo "\nMEDIUM TESTS: \n" >> raport1.txt

	./main firtsMtest.txt secondMtest.txt >> raport1.txt

	@echo "\nLARGE TESTS: \n" >> raport1.txt

	./main firstLtest.txt secondLtest.txt >> raport1.txt


clean_files:
	rm -f *test.txt

copy:
	cp -r ../files/*test.txt ../zad1

clean:
	rm -f main
	make clean_files

