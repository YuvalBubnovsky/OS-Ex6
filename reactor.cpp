//
// Created by yuval on 6/3/22.
//

#include "reactor.hpp"

namespace ex6{
    Reactor::Reactor(){
        this->fd = nullptr;
        this->handler = nullptr;
    }

    void* Reactor::newReactor(){
        Reactor();
    }

    void Reactor::add_handler(int* fd, void *(handler)(void*)){
        this->fd = fd;
        this->handler = handler;
        pthread_create(&this->thread, nullptr, handler, (void*)fd);
    }

    void Reactor::remove_handler(){
        this->fd = nullptr;
        this->handler = nullptr;
        pthread_cancel(this->thread);
       // this->~Reactor(); // Is this needed here?
    }

    Reactor::~Reactor() {
        delete this;
    }
}