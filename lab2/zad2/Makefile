CC = gcc -Wall -g

all: test clean

test:
	$(CC) main.c -o main

	@echo "SMALL TESTS: \n" >> raport2.txt

	./main a small2test.txt >> raport2.txt

	@echo "\nMEDIUM TESTS: \n" >> raport2.txt

	./main b medium2test.txt >> raport2.txt

	@echo "\nLARGE TESTS: \n" >> raport2.txt

	./main c large2test.txt >> raport2.txt


clean_files:
	rm -f *2test.txt

copy:
	cp -r ../files/*2test.txt ../zad2

clean:
	rm -f main
	make clean_files

