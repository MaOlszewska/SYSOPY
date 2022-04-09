CC = gcc -Wall -g

all:
	make test
	make clean


test:
	$(CC) main.c -o main
	$(CC) consumer.c -o consumer
	$(CC) producent.c -o producent
	./main pipe output.txt fileA.txt fileB.txt fileC.txt fileD.txt fileE.txt


clean:
	rm -f main consumer producent pipe 