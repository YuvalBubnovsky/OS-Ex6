//
// Created by yuval on 5/31/22.
//

#pragma once
#ifndef EX6_NODE_HPP
#define EX6_NODE_HPP

namespace ex6
{
    template <class T>
    class Node
    {
    private:
        T *prev;
        T *next;
        T value;

    public:
        T _getValue(){
            return this->value;
        }
        T *_getNext(){
            return this->next;
        }
        T *_getPrev(){
            return this->prev;
        }
        void _setValue(T &new_val){
            this->value=new_val;
        }
        void _setNext(T *new_next){
            this->next=new_next;
        }
        void _setPrev(T *new_prev){
            this->prev=new_prev;
        }
    };
}

#endif //EX6_NODE_HPP
