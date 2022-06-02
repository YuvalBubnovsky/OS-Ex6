//
// Created by yuval on 6/1/22.
//


/* Q4 Answer:
 * When working with multiple threads, this guard cannot protect strtok
 * since multiple threads may be calling strtok with different string.
 * strtok will only keep track of the last string and eventually will
 * return the wrong result
 */

#ifndef EX6_GUARD_HPP
#define EX6_GUARD_HPP

#include <iostream>
#include <stdexcept>
#include <pthread.h>

// Taken straight from the file in the moodle module
// TODO: answer question 4

namespace ex6{
    class Guard {
    private:
        pthread_mutex_t *lock;
    public:
        Guard(pthread_mutex_t* lock) {
            this->lock = lock;
            pthread_mutex_lock(this->lock);
        }
        ~Guard() { pthread_mutex_unlock(this->lock); }
    };
}

#endif //EX6_GUARD_HPP
