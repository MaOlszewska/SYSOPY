CC = gcc -Wall -g

all:
	make test
	make clean


test:
	$(CC) main.c -o main

	./main data
	./main nadawca
	./main sysopy@gmail.com SYSOPY SysopySaSuperowe

clean:
	rm -f main