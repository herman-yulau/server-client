# makefile for helloworld

CC = gcc
SOURCE = client.c
S_SOURCE = server.c
OUT = server client

all: server client
	
server:
	$(CC) $(S_SOURCE) -o server

client:
	$(CC) $(SOURCE) -o client

clean:
	rm -rf *.o $(OUT)
