all: server client

server: server.c
	gcc -g -Wall server.c -o server -lrt

client: client.c
	gcc -g -Wall client.c -o client -lrt

clean:
	rm server client