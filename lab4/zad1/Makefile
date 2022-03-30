CC = gcc -Wall -g

all:
	make compile-child
	make test
	make clean

compile-child:
	$(CC) child.c -o child

test:
	$(CC) fork.c -o fork

	@echo "-------------------------FORK TEST-------------------- \n\n" >> raport1.txt
	./fork ignore >> raport1.txt
	@echo "\n----------------\n" >> raport1.txt
	./fork handler >> raport1.txt
	@echo "\n----------------\n" >> raport1.txt
	./fork mask >> raport1.txt
	@echo "\n----------------\n" >> raport1.txt
	./fork pending >> raport1.txt

	$(CC) exec.c -o exec
	@echo "\n\n -------------------------EXEC TEST-------------------- \n\n" >> raport1.txt
	./exec ignore >> raport1.txt
	@echo "\n----------------\n" >> raport1.txt
	./exec mask >> raport1.txt
	@echo "\n----------------\n" >> raport1.txt
	./exec pending >> raport1.txt

clean:
	rm -f fork exec child