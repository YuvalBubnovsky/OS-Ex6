//
// Created by yuval on 6/1/22.
//

#ifndef EX6_SINGLETON_HPP
#define EX6_SINGLETON_HPP

#include <iostream>
#include <stdexcept>
#include <pthread.h>
#include "guard.hpp"

// used https://stackoverflow.com/questions/42555782/singleton-with-template for reference

namespace ex6{
    template<typename T>
    class Singleton{
    private:
        static T* _instance = nullptr; // is this okay?
        static pthread_mutex_t _mutex;
        Singleton()=default;
        ~Singleton() {
            delete this->_instance;
            this->_instance = nullptr;
        }
        Singleton& operator=(const Singleton&){}
        Singleton(const Singleton&)= default;

    public:
        static T* Instance(){
            if(_instance== nullptr){
                pthread_mutex_lock(&_mutex);
                if(_instance==nullptr){
                    _instance = new T();
                }
                pthread_mutex_unlock(&_mutex);
            }
            return _instance;
        }
    };
}

#endif //EX6_SINGLETON_HPP
