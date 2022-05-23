all: compile clean

compile:
	gcc -g -Wall main.c -o main -lm -pthread

run: compile
	@echo "HANDSMAT numbers\n\n" >> Times.txt
	./main 1 numbers handsmat.ascii.pgm outputHands.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 2 numbers handsmat.ascii.pgm outputHands.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 4 numbers handsmat.ascii.pgm outputHands.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 8 numbers handsmat.ascii.pgm outputHands.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 16 numbers handsmat.ascii.pgm outputHands.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt

	@echo "LENA numbers \n\n" >> Times.txt

	./main 1 numbers lena.ascii.pgm outputlena.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 2 numbers lena.ascii.pgm outputlena.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 4 numbers lena.ascii.pgm outputlena.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 8 numbers lena.ascii.pgm outputlena.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 16 numbers lena.ascii.pgm outputlena.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt

	@echo "HANDSMAT block \n\n" >> Times.txt
	./main 1 block handsmat.ascii.pgm outputHands.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 2 block handsmat.ascii.pgm outputHands.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 4 block handsmat.ascii.pgm outputHands.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 8 block handsmat.ascii.pgm outputHands.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 16 block handsmat.ascii.pgm outputHands.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >>Times.txt

	@echo "LENA block \n\n" >> Times.txt

	./main 1 block lena.ascii.pgm outputlena.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 2 block lena.ascii.pgm outputlena.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 4 block lena.ascii.pgm outputlena.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 8 block lena.ascii.pgm outputlena.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
	./main 16 block lena.ascii.pgm outputlena.ascii.pgm >> Times.txt
	@echo "\n------------------------\n" >> Times.txt
clean:
	rm main Times.txt
