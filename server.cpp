//
// Created by yuval on 5/31/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

#include <utility>
#include "queue.hpp"
#include "active_object.hpp"

using namespace std;
using namespace ex6;

#define PORT "3490" // the port users will be connecting to

#define BACKLOG 10 // how many pending connections queue will hold

struct fs
{
    string val;
    int *fd;
    fs(string val, int *fd) : val(std::move(val)), fd(fd) {}
};

// Defining global queue and sending socket for client globally
int new_sock = 0;

Queue<> *queue_1;
Queue<> *queue_2;
Queue<> *queue_3;
activeObject<> *AO_1;
activeObject<> *AO_2;
activeObject<> *AO_3;

void caesar_cypher(void *data)
{
    // TODO:
    char ch;
    int key = 1;
    auto *node = (Queue<>::Node<> *)data;
    string *msg = &node->value;
    *msg = "a";

    /*
    char *msg = node->value.c_str();

    for (int i = 0; msg[i] != '\0'; ++i)
    {
        ch = msg[i];

        // encrypt for lowercase letter
        if (ch >= 'a' && ch <= 'z')
        {
            ch = ch + key;
            if (ch > 'z')
            {
                ch = ch - 'z' + 'a' - 1;
            }
            msg[i] = ch;
        }
        // encrypt for uppercase letter
        else if (ch >= 'A' && ch <= 'Z')
        {
            ch = ch + key;
            if (ch > 'Z')
            {
                ch = ch - 'Z' + 'A' - 1;
            }
            msg[i] = ch;
        }
    }*/
}

void convert(void *data)
{
    auto *node = (Queue<>::Node<> *)data;
    string *msg = &node->value;
    *msg = "A";
}
    // TODO
    /*char ch;
    //int key = 1;
    char *msg = (char *)data;
    for (int i = 0; msg[i] != '\0'; ++i)
    {
        ch = msg[i];
        // convert for lowercase letter
        if (ch >= 'a' && ch <= 'z')
        {
            ch = (ch - 'a') + 'A';
            msg[i] = ch;
        }
        // convert for uppercase letter
        else if (ch >= 'A' && ch <= 'Z')
        {
            ch = (ch - 'A') + 'a';
            msg[i] = ch;
        }
    }
}*/

void sendto(void *data)
{
    auto *node = (Queue<>::Node<> *)data;
    string *msg = &node->value;
    string s = *msg;
    int n = s.length();
    char to_send[n + 1];
    strcpy(to_send, s.c_str());
    to_send[n] = '\0';
    send(new_sock, to_send, strlen(to_send), 0);
}

void enQ_middle(void *x)
{
    queue_2->m_enQ((string *)x);
    cout << *(string *)x << endl;
}

void enQ_end(void *x)
{
    queue_3->m_enQ((string *)x);
    cout << *(string *)x << endl;
}

void *sock_thread(void *arg) /* ***************** THREAD HANDLER ***************** */
{
    /**
     * arg is a fd for a socket (client)
     */
    int n;
    char buffer[2048];
    //char **args;
    new_sock = *((int *)arg);
    bzero(buffer, 2048);
    printf("DEBUG: New connection from %d\n", new_sock); // DEBUG ONLY
    sleep(1);


    n = recv(new_sock, &buffer, sizeof(buffer), 0);
    buffer[n] = '\0';
    string s = buffer;
    queue_1->m_enQ(&s); // TODO: potential rvalue reference error!

    close(new_sock);
    pthread_exit(nullptr);
}

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main()
{

    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    // Initializing the queues
    queue_1 = queue_1->createQ();
    queue_2 = queue_2->createQ();
    queue_3 = queue_3->createQ();

    // Initializing the active objects
    AO_1 = new activeObject<>(queue_1, &caesar_cypher, &enQ_middle);
    AO_2 = new activeObject<>(queue_2, &convert, &enQ_end);
    AO_3 = new activeObject<>(queue_3, &sendto, nullptr);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(nullptr, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != nullptr; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == nullptr)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, nullptr) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    pthread_t new_thread[10]; // We need to be able to server 10 connections at the same time
    int i = 0;
    while (true)
    { // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);

        // create a new thread, assign it to our thread array and send the thread_id and socket FD to out thread handler
        if (pthread_create(&new_thread[i++], nullptr, sock_thread, &new_fd) != 0)
        {
            printf("ERROR: Failed To Create Thread!\n");
        }

        // Loop over our threads array and join all completed threads, freeing up resources
        if (i >= 10)
        {
            i = 0;
            while (i < 10)
            {
                pthread_join(new_thread[i++], nullptr);
            }
            i = 0;
        }
    }

    return 0;
}