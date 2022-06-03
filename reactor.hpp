//
// Created by yuval on 6/3/22.
//

#ifndef EX6_REACTOR_HPP
#define EX6_REACTOR_HPP

#include <pthread.h>
#include <stdlib.h>

namespace ex6 {
    class Reactor {
    private:
        Reactor();
        int* fd;
        pthread_t thread{};
        void*(*handler)(void*);


    public:
        void* newReactor();

        virtual ~Reactor();

        void add_handler(int* fd, void *(handler)(void*));

        void remove_handler();

        int getFD();

    };
}


#endif //EX6_REACTOR_HPP
