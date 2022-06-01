//
// Created by yuval on 6/1/22.
//

#pragma once
#ifndef EX6_ACTIVE_OBJECT_HPP
#define EX6_ACTIVE_OBJECT_HPP

#include <iostream>
#include <stdexcept>
#include <pthread.h>
#include <unistd.h>
#include "queue.hpp"

namespace ex6 {
    template<class T = string>
    class activeObject {
    private:
        Queue<T> *queue;
        pthread_t thread;

        void *(*for_each)(void *value);

        void *(*finally)(void *value);

    public:

        // TODO: add newAO & destroyAO
        activeObject() = default;

        activeObject(Queue<T> *q, void *(*func1)(void *value), void *(*func2)(void *value)) : queue(q), for_each(func1),
                                                                                              finally(func2) {
            pthread_create(&thread, nullptr, &activeObject::handler, &(*this));
        }

        ~activeObject(){
            pthread_cancel(thread);
            // TODO: is anything more needed?
        }

        void* handler(activeObject<T> *AO){
                auto q = (*this->queue);
                while(!q.m_isEmpty()){
                    auto *item = q.m_deQ();
                    if(item != nullptr){
                        void* function1_result = AO->for_each(item);
                        void* final_result = AO->finally(function1_result);
                    }
            }
        }
    };
}

#endif //EX6_ACTIVE_OBJECT_HPP
