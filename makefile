CC = g++
CFLAGS = -g -Wall


all: server

server: server.cpp
	$(CC) $(CFLAGS) -o server server.cpp

