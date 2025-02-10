//
// Created by jfngsea on 2/8/25.
//

#ifndef MYQUEUE_H
#define MYQUEUE_H

#include <stdexcept>

#include "Queue.h"

template <typename T>
class MyQueue : public Queue<T> {
    int sz;
    T* data;

    int pop_idx =0;
    int elem_count=0;

public:
    explicit MyQueue(int size) {
        this->sz = size;
        data = new T[size];
    }
    ~MyQueue() {
        delete[] data;
    }

    void push(T item) override {
        if (elem_count == sz) {
            data[pop_idx] = item;
            pop_idx = (pop_idx + 1) % sz;
        } else {
            data[elem_count++] = item;
        }
    }

    T pop() override {
        if (elem_count == 0) {
            //wait
            throw std::out_of_range("MyQueue::pop empty");
        }

        T elem = data[pop_idx];
        elem_count--;
        pop_idx = (pop_idx + 1) % sz;

        return elem;
    }

    T popWithTimeout(int milliseconds) override {
        return pop();
    }

    int count() override {
        return elem_count;
    }

    int size() override {
        return this->sz;
    }

};

#endif //MYQUEUE_H
