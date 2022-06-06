player1:
	gcc -Wall -g player.c -o player -lpthread -lm
	./player "Bob" unix socket

player2:
	gcc -Wall -g player.c -o player -lpthread -lm
	./player "Alice" unix socket

server:
	gcc -Wall -g server.c -o server -lpthread -lm
	./server 7001 socket

clean:
	rm server player socket test

test: test.c common.c
	gcc -Wall -g test.c -o test
	./test