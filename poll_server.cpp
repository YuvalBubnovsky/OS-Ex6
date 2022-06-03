/*
** pollserver.c -- a cheezy multiperson chat server
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

#include "reactor.hpp"
using namespace std;

#define PORT "3490" // Port we're listening on
#define MAX_SIZE_ALLOWED 1024

int newfd;
Reactor* reactor;
void *newReactor(); // prototype for compiler

// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

// Return a listening socket
int get_listener_socket() {
    int listener; // Listening socket descriptor
    int yes = 1;  // For setsockopt() SO_REUSEADDR, below
    int rv;

    struct addrinfo hints{}, *ai, *p;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(nullptr, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }

    for (p = ai; p != nullptr; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    freeaddrinfo(ai); // All done with this

    // If we got here, it means we didn't get bound
    if (p == nullptr) {
        return -1;
    }

    // Listen
    if (listen(listener, 10) == -1) {
        return -1;
    }

    return listener;
}

// Add a new file descriptor to the set
void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size) {
    // If we don't have room, add more space in the pfds array
    if (*fd_count == *fd_size) {
        *fd_size *= 2; // Double it

        *pfds = (pollfd *) realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read

    (*fd_count)++;
}

// Remove an index from the set
__attribute__((unused)) void del_from_pfds(struct pollfd pfds[], int i, int *fd_count) {
    // Copy the one from the end over this one
    pfds[i] = pfds[*fd_count - 1];

    (*fd_count)--;
}

void *handler(void *client_fd) {

    int *p_new_fd = (int *) client_fd;
    int new_fd = *p_new_fd;

    const char *welcome_message = "Welcome to the chat server!\n";
    send(new_fd, welcome_message, strlen(welcome_message), 0);

    char buffer[MAX_SIZE_ALLOWED];

    while (true) {
        int bytes_received = recv(new_fd, buffer, sizeof(buffer), 0);

        if (bytes_received <= 0) {
            perror("recv");
            if(reactor != nullptr) {
                reactor->RemoveHandler(reactor, &newfd);
            }
            printf("DEBUG: Client Disconnected!");
            close(new_fd); // Bye!
            break;
        } else {
            const char* server_message = "This Is The Server Answering You! Nothin Special Just Demonstrating Communication!\n";
            send(new_fd, server_message, strlen(server_message), 0);
        }
    }
    return nullptr;
}

int main(void) {
    int listener; /* Listening socket descriptor */

    int newfd;                          /* New connection socket descriptor */
    struct sockaddr_storage remoteaddr{}; /* Client address */
    socklen_t addrlen;

    //char buf[256]; // Buffer for client data

    char remoteIP[INET6_ADDRSTRLEN];

    // Start off with room for 5 connections
    // (We'll realloc as necessary)
    int fd_count = 0;
    int fd_size = 5;
    struct pollfd *pfds = (pollfd *) malloc(sizeof *pfds * fd_size);

    // Set up and get a listening socket
    listener = get_listener_socket();

    if (listener == -1) {
        fprintf(stderr, "ERROR: getting listening socket\n");
        exit(1);
    }
    printf("PollServer: Waiting for connections...\n");

    // Add the listener to set
    pfds[0].fd = listener;
    pfds[0].events = POLLIN; // Report ready to read on incoming connection

    fd_count = 1; // For the listener
    // Main loop
    while (true) {
        int poll_count = poll(pfds, fd_count, -1);

        if (poll_count == -1) {
            perror("ERROR: poll");
            exit(1);
        }

        // Run through the existing connections looking for data to read
        for (int i = 0; i < fd_count; i++) {

            // Check if someone's ready to read
            if (pfds[i].revents & POLLIN) { // We got one!!

                if (pfds[i].fd == listener) {
                    // If listener is ready to read, handle new connection

                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,
                                   (struct sockaddr *) &remoteaddr,
                                   &addrlen);

                    if (newfd == -1) {
                        perror("ERROR: failed to accept");
                    } else {
                        add_to_pfds(&pfds, newfd, &fd_count, &fd_size);

                        printf("PollServer: new connection from %s on "
                               "socket %d\n",
                               inet_ntop(remoteaddr.ss_family,
                                         get_in_addr((struct sockaddr *) &remoteaddr),
                                         remoteIP, INET6_ADDRSTRLEN),
                               newfd);
                        auto *reactor = (Reactor *) newReactor();
                        InstallHandler(reactor, &handler, &newfd);
                    }
                }
            } // END got ready-to-read from poll()
        }     // END looping through file descriptors
    }         // END for(;;)--and you thought it would never end!

    return 0;
}