CC = gcc -Wall -g

all:
	make test
	make clean


test:
	$(CC) main.c -o main

	./main SIGINFO >> raport2.txt
	@echo "\n----------------\n" >> raport2.txt
	./main NOCLDSTOP >> raport2.txt
	@echo "\n----------------\n" >> raport2.txt
	./main RESETHAND >> raport2.txt

clean:
	rm -f main