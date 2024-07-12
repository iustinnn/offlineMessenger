#fisier folosit pentru compilarea serverului&clientului TCP iterativ

all:
	gcc -pthread server.c -o server -lsqlite3
	gcc -pthread client.c -o client -lsqlite3
clean:
	rm -f client server
