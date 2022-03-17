CC = gcc -Wall -g

all:
	make test
	make clean

test:
	$(CC) main.c -o main
	./main dir1 >> raport3.txt

clean:
	rm -f main