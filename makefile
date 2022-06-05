CC = g++
CFLAGS = -g -Wall -fpic
HEADERS=$(wildcard *.hpp)
LDFLAGS=-pthread

.PHONY: clean all lib

all: server client poll lib

lib: server.so client.so poll_server.so

server: server.o
	$(CC) $(CFLAGS) -o server server.o $(LDFLAGS)

server.so: server.o
	$(CC) -shared $(CFLAGS) -o server.so server.o $(LDFLAGS)

server.o: server.cpp
	$(CC) $(CFLAGS) -c -o server.o server.cpp $(LDFLAGS)

client: client.o
	$(CC) $(CFLAGS) -o client client.cpp $(LDFLAGS)

client.so: client.o
	$(CC) -shared $(CFLAGS) -o client.so client.o $(LDFLAGS)

client.o: client.cpp
	$(CC) $(CFLAGS) -c -o client.o client.cpp $(LDFLAGS)

poll: poll_server.o
	$(CXX) $(CFLAGS) poll_server.o reactor.hpp -o poll_server $(LDFLAGS)

poll_server.so: poll_server.o
	$(CXX) -shared $(CFLAGS) -o poll_server.so poll_server.o reactor.hpp $(LDFLAGS)

poll_server.o: $(HEADERS)  poll_server.cpp
	$(CXX) $(CFLAGS) -c poll_server.cpp $(LDFLAGS)


clean:
	rm -f server client poll_server  *.o *.a *.so
