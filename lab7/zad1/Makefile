all: compile clean

compile:
	gcc -g -Wall main.c -o main
	gcc -g -Wall pizza.c -o pizza
	gcc -g -Wall deliver.c -o deliver

run:
	./main 4 4
clean:
	rm pizza main deliver