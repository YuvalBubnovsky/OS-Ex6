//
// Created by yuval on 5/31/22.
//

#pragma once
#ifndef EX6_QUEUE_H
#define EX6_QUEUE_H

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <string>
using std::string;

namespace ex6
{
    template <class T = string>
    class Queue
    {
    public:
        template <class V = string>
        class Node{ // acts like struct
        public:
            V value;
            Node<V>* next = nullptr;
            Node<V>* prev = nullptr;

        public:
            explicit Node(V& val) : value(val) {}
            void _setPrev(Node<V>* new_prev) {this->prev = new_prev;}
            void _setNext(Node<V>* new_next) {this->next = new_next;}
            Node<V>* _getPrev() {return this->prev;}
            Node<V>* _getNext() {return this->next;}
        };

    private:
        size_t m_size;
        Node<T> *m_head;
        Node<T> *m_tail;

        pthread_mutex_t lock{};
        pthread_cond_t condi{}; // GW2 reference

    public:

        Queue<T>* createQ(){
            auto* q = new Queue();
            return q;
        }

        void destroyQ(){
            ~Queue();
        }

        Queue(){
            m_head = nullptr;
            m_tail = nullptr;
            m_size = 0;
            pthread_mutex_init(&lock, nullptr);
            pthread_cond_init(&condi, nullptr);
        }

        ~Queue() {
            // TODO: free all structs
            // TODO: check if any threads are using the queue before destroying
            pthread_mutex_destroy(&lock);
            pthread_cond_destroy(&condi);
        }

        bool m_isEmpty(){
            return m_getHead() == nullptr;
        }

        Node<T>* m_getHead()
        {
            return this->m_head;
        }

        Node<T>* m_getTail()
        {
            return this->m_tail;
        }

        size_t m_getSize(){
            return this->m_size;
        }

        void m_setHead(Node<T> *new_head)
        {
            this->m_head=new_head;
        }

        void m_setTail(Node<T> *new_tail)
        {
            this->m_tail=new_tail;
        }

        void m_setSize(size_t new_size)
        {
            this->m_size=new_size;
        }

        void m_enQ(T* text)
        {
            //TODO: code isnt valid.
            pthread_mutex_lock(&lock);
            auto *node = new Node<T>(*text); // 'new' is a memory allocation!
            if (this->m_size == 0)
            {
                this->m_head = node;
                this->m_tail = node;
            }
            else
            {
                node->_setPrev(this->m_tail);
                this->m_tail->_setNext(node);
                this->m_tail = node;
            }
            this->m_size += 1;
            pthread_cond_signal(&condi); // https://stackoverflow.com/questions/16522858/understanding-of-pthread-cond-wait-and-pthread-cond-signal
            pthread_mutex_unlock(&lock);
        }

        void *m_deQ()
        {
            pthread_mutex_lock(&lock);
            while(m_isEmpty()){
               pthread_cond_wait(&condi,&lock);
            }
            Node<T> *node = this->m_tail;
            Node<T> *new_tail = this->m_tail->_getPrev();
            if(new_tail!=nullptr) {
                new_tail->_setNext(nullptr);
                this->m_tail = new_tail;
            }
            else{
                this->m_head = nullptr;
                this->m_tail = nullptr;
            }
            this->m_size -= 1;
            pthread_mutex_unlock(&lock);
            return (void *)&(*node);
        }

        void m_ToString()
        {
            // TODO: implement if needed
        }
    };
}

#endif //EX6_QUEUE_H
