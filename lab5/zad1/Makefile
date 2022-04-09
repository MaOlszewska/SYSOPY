CC = gcc -Wall -g

all:
	make test
	make clean


test:
	$(CC) main.c -o main
	./main test.txt


clean:
	rm -f main