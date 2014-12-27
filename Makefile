all:
	gcc -Wall -o client client.c -pthread
	gcc -Wall -o server server.c -pthread
