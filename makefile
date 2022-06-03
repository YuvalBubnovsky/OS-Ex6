CC = g++
CFLAGS = -g -Wall
HEADERS=$(wildcard *.hpp)
LDFLAGS=-pthread

.PHONY: clean all

all: server client poll

server: server.cpp
	$(CC) $(CFLAGS) -o server server.cpp $(LDFLAGS)

client: client.cpp
	$(CC) $(CFLAGS) -o client client.cpp $(LDFLAGS)

poll: poll_server.o reactor.o
	$(CXX) $(CFLAGS) poll_server.o reactor.o -o poll_server $(LDFLAGS)

poll_server.o: $(HEADERS)  poll_server.cpp
	$(CXX) $(CFLAGS) -c poll_server.cpp $(LDFLAGS)

reactor.o: $(HEADERS)  reactor.cpp
	$(CXX) $(CFLAGS) -c reactor.cpp

clean:
	rm -f server client poll_server  *.o *.a