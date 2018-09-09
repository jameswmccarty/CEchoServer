#Makefile for simple TCP echo server

CC = gcc
CFLAGS= -ansi -pedantic -Werror -Wall -O2
LFLAGS=

all: client server

TCPSocketLib: TCPSocketLib.c 
	$(CC) $(CFLAGS) TCPSocketLib.c -c -o TCPSocketLib.o

client:	client.c TCPSocketLib
	$(CC) $(CFLAGS) client.c TCPSocketLib.o -o client

server: server.c TCPSocketLib
	$(CC) $(CFLAGS) server.c TCPSocketLib.o -o server

clean:
	\rm client server *.o
