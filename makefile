CC = g++
CFLAGS = -g -Wall
LDFLAGS=-pthread

.PHONY: clean all

all: server client

server: server.cpp
	$(CC) $(CFLAGS) -o server server.cpp $(LDFLAGS)

client: client.cpp
	$(CC) $(CFLAGS) -o client client.cpp $(LDFLAGS)

clean:
	rm -f server client *.o *.a