CC = g++
CFLAGS = -g -Wall
LDFLAGS=-pthread

.PHONY: clean all

all: server

server: server.cpp
	$(CC) $(CFLAGS) -o server server.cpp $(LDFLAGS)

clean:
	rm -f server *.o *.a