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

namespace ex6
{
    template <class T = string>
    class activeObject
    {
    private:
        Queue<T> *queue;
        pthread_t thread;

        void (*for_each)(void *value){};

        void (*finally)(void *value){};

    public:
        void handler()
        {
            auto q = (*this->queue);
            while (1)
            {
                if (!q.m_isEmpty())
                {
                    auto *item = q.m_deQ();
                    if (item != nullptr)
                    {
                        this->for_each(item);
                        this->finally(item);
                    }
                }
            }
        }

        // TODO: add newAO & destroyAO
        activeObject() = default;

        activeObject(Queue<T> *q, void (*func1)(void *value), void (*func2)(void *value)) : queue(q), for_each(func1),
                                                                                            finally(func2)
        {
            pthread_create(&thread, nullptr, &handler, nullptr);
        }

        ~activeObject()
        {
            pthread_cancel(thread);
            // TODO: is anything more needed?
        }
    };
}

#endif // EX6_ACTIVE_OBJECT_HPP
