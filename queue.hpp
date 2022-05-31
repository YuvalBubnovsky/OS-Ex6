//
// Created by yuval on 5/31/22.
//

#pragma once
#ifndef EX6_QUEUE_H
#define EX6_QUEUE_H

#include <iostream>
#include <string>
using std::string;

namespace ex6
{
    template <class T = string>
    class Queue
    {
    public:
        template <class V = string>
        struct Node{
            V value;
            Node<V>* next;
            Node<V>* prev;
        };

    private:
        size_t size;
        Node<T> *head;
        Node<T> *tail;

    public:
        Queue(){
            this->head=NULL;
            this->tail=NULL;
            this->size=0;
        }

        ~Queue()= default;

        Node<T>* getHead(){
            return this->head;
        }
        Node<T>* _getTail(){
            return this->tail;
        }
        size_t _getSize(){
            return this->size;
        }

        void _setHead(Node<T> *new_head){
            this->head=new_head;
        }
        void _setTail(Node<T> *new_tail){
            this->tail=new_tail;
        }
        void _setSize(size_t new_size){
            this->size=new_size;
        }

        void _enQ(T text){
            Node<T> *node = new Node<T>(text); // 'new' is a memory allocation!
            if (this->size == 0)
            {
                this->head = node;
                this->tail = node;
            }
            else
            {
                node->_setPrev(this->tail);
                this->tail->_setNext(node);
                this->tail = node;
            }
            this->size += 1;
        }

        void _deQ(){
            Node<T> *node = this->tail;
            Node<T> *new_tail = this->tail->_getPrev();
            new_tail->_setNext(NULL);
            this->tail = new_tail;
            return node->value;
        }

        void _ToString(){
            // TODO: implement if needed
        }
    };
}

#endif //EX6_QUEUE_H
