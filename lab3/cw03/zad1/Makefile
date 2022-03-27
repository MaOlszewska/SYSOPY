CC = gcc -Wall -g

all:
	make test
	make clean

test:
	$(CC) main.c -o main
	@echo "--------5 procesów--------\n" >> raport1.txt
	./main 5 >> raport1.txt
	@echo "--------10 procesów--------- \n" >> raport1.txt
	./main 10 >> raport1.txt
clean:
	rm -f main