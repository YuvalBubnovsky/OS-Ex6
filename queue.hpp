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
        size_t m_size;
        Node<T> *m_head;
        Node<T> *m_tail;

    public:
        Queue(){
            m_head = nullptr;
            m_tail = nullptr;
            m_size = 0;
        }

        ~Queue()= default;

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

        void m_enQ(T text)
        {
            auto *node = new Node<T>(text); // 'new' is a memory allocation!
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
        }

        void m_deQ()
        {
            Node<T> *node = this->m_tail;
            Node<T> *new_tail = this->m_tail->_getPrev();
            new_tail->_setNext(NULL);
            this->m_tail = new_tail;
            return node->value;
        }

        void m_ToString()
        {
            // TODO: implement if needed
        }
    };
}

#endif //EX6_QUEUE_H
