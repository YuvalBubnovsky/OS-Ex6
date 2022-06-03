//
// Created by yuval on 6/1/22.
//

#ifndef EX6_SINGLETON_HPP
#define EX6_SINGLETON_HPP

#include <iostream>
#include <stdexcept>
#include <pthread.h>

// used https://stackoverflow.com/questions/42555782/singleton-with-template for reference

namespace ex6{
    template<typename T>
    class Singleton{
    private:
        __attribute__((unused)) static Singleton* _instance;
        __attribute__((unused)) static pthread_mutex_t _mutex;
        __attribute__((unused)) T _value;
        explicit Singleton(T junk){
            _value = junk;
        }
        ~Singleton() {
            delete this->_instance;
            this->_instance = nullptr;
        }
        Singleton& operator=(const Singleton&){}
        Singleton(const Singleton&)= default;

    public:
        static Singleton* Instance(T junk){
            if(_instance== nullptr){
                pthread_mutex_lock(&_mutex);
                if(_instance==nullptr){
                    _instance = new Singleton(junk);
                }
                pthread_mutex_unlock(&_mutex);
            }
            return _instance;
        }
    };
    template<typename T> Singleton<T> *Singleton<T>::_instance = nullptr;
    template<typename T> pthread_mutex_t Singleton<T>::_mutex = PTHREAD_MUTEX_INITIALIZER;
}

#endif //EX6_SINGLETON_HPP
