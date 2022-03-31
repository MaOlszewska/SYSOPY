CC = gcc -Wall -g

all:
	make test
	make clean

compile:
	$(CC) catcher.c -o catcher
	$(CC) sender.c -o sender

test:
	make compile


	@./catcher SIGRT &
	@./sender `pidof -s catcher` 1000 SIGRT



clean:
	rm -f catcher sender