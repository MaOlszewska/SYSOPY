CC = gcc -Wall -g

all:
	make test
	make clean

test:
	$(CC) main.c -o main
	@echo "--------50 procesów, szerokosć 0.02--------" >> raport2.txt
	./main 0.02f 50 >> raport2.txt
	@echo "--------10 procesów, szerokość 0.02---------" >> raport2.txt
	./main 0.02f 10 >> raport2.txt
	@echo "--------5 procesów, szerokosć 0.1--------" >> raport2.txt
	./main 0.1f 5 >> raport2.txt
	@echo "--------10 procesów, szerokość 0.1---------" >> raport2.txt
	./main 0.1f 10 >> raport2.txt
	@echo "--------5 procesów, szerokosć 0.01--------" >> raport2.txt
	./main 0.01f 5 >> raport2.txt
	@echo "--------10 procesów, szerokość 0.01---------" >> raport2.txt
	./main 0.01f 10 >> raport2.txt
	@echo "--------50 procesów, szerokosć 0.01--------" >> raport2.txt
	./main 0.01f 50 >> raport2.txt
	@echo "--------100 procesów, szerokość 0.01---------" >> raport2.txt
	./main 0.01f 100 >> raport2.txt
	@echo "--------100 procesów, szerokość 0.0001---------" >> raport2.txt
	./main 0.0001f 100 >> raport2.txt
	@echo "--------100 procesów, szerokość 0.001---------" >> raport2.txt
	./main 0.001f 100 >> raport2.txt
	@echo "--------100 procesów, szerokosć 0.01--------" >> raport2.txt
	./main 0.01f 100 >> raport2.txt
	@echo "--------10000 procesów, szerokosć 0.0001--------" >> raport2.txt
	./main 0.0001f 10000 >> raport2.txt



clean:
	rm -f main w*.txt