CC = gcc -Wall -g

all:
	make test
	make clean

test:
	$(CC) main.c -o main

	./main dir1 Sysopy 6
	./main dir1 agh 6
	./main dir1 ok 6

clean:
	rm -f main