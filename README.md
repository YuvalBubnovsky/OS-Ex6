# OS-Ex6

## Implemented in this exercise
* Thread-Safe Queue
* Thread-Safe application using Active-Object design pattern to implement a function pipline
* Guard
* Singleton pattern
* Reactor pattern using a poll server

## How to run
    
    cd /path/to/ex6/ && make
    ./server - will run the server
    ./poll_server - will run the poll server
    ./client localhost - will run the client and connect to the running server
Regular server will recieve strings from user to run through a pipeline as specified in the exercise details.<br>
Poll server will recieve strings from user and use a reactor to install function handler and send a message back to user.

### Included Test Functions
* guard_test
* singleton_test
