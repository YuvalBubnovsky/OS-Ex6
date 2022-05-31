//
// Created by yuval on 5/31/22.
//

#pragma once
#ifndef EX6_QUEUE_H
#define EX6_QUEUE_H

#include <iostream>
#include <string>
#include "node.hpp"
using std::string;

namespace ex6
{
    template <class T = string>
    class Queue
    {
    private:
        size_t size;
        Node<T> *head;
        Node<T> *tail;

    public:
        struct Node{
            T value;
            Node* next;
            Node* prev;
        };

    public:
        Queue();
        ~Queue();

        Node* getHead(){
            return this->head;
        }
        Node* _getTail(){
            return this->tail;
        }
        size_t _getSize(){
            return this->size;
        }

        void _setHead(Node *new_head){
            this->head=new_head;
        }
        void _setTail(Node *new_tail){
            this->tail=new_tail;
        }
        void _setSize(size_t new_size){
            this->size=new_size;
        }

        void enQ();
        void deQ()
        void _ToString();
    };
}

#endif //EX6_QUEUE_H
