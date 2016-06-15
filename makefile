# makefile for helloworld

CC = gcc
SOURCE = client.c
S_SOURCE = server.c
OUT = server client

all: server.o client.o
	$(CC) -o server server.o
	$(CC) -o client client.o
	
server.o:
	$(CC) -c $(S_SOURCE)

client.o:
	$(CC) -c $(SOURCE)

clean:
	rm -rf *.o $(OUT)
