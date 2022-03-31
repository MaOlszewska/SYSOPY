CC = gcc -Wall -g

all:
	make test
	make clean

compile:
	$(CC) catcher.c -o catcher
	$(CC) sender.c -o sender

test:
	make compile


	@./catcher KILL &
	@./sender `pidof -s catcher` 1000 KILL
	@./catcher SIGQUEUE &
	@./sender `pidof -s catcher` 100 SIGQUEUE
	@./catcher SIGRT &
	@./sender `pidof -s catcher` 100 SIGRT



clean:
	rm -f catcher sender