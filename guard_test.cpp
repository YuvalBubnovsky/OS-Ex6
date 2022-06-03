//
// Created by yuval on 6/3/22.
//

#include <iostream>
#include <pthread.h>
#include "guard.hpp"

using namespace std;
using namespace ex6;


int counter = 0; // global variable to increment
pthread_mutex_t mutex; // mutex to protect the global variable

void increment() {
    Guard guard(&mutex); // lock the mutex using uard
    counter++;
    cout << "counter: " << counter << ", done by thread: " << pthread_self() << endl;
}

void* runner(void* arg){ // thread function
    for(int i = 0; i < 10; i++){
        increment();
    }
}

int main(){
    pthread_mutex_init(&mutex, nullptr); // initialize the mutex
    pthread_t t1, t2;
    int num1 = 0, num2 = 5;
    pthread_create(&t1, nullptr, runner, &num1); // create thread 1
    pthread_create(&t2, nullptr, runner, &num2); // create thread 2
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    return 0;
}
