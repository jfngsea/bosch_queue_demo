//
// Created by jfngsea on 2/8/25.
//

#ifndef MYQUEUE_H
#define MYQUEUE_H

#include <stdexcept>
#include <mutex>
#include <condition_variable>

#include "Queue.h"

template <typename T>
class MyQueue : public Queue<T> {
    int sz;
    T* data;

    int pop_idx =0;
    int push_idx =0;
    int elem_count=0;

    std::mutex m;
    std::condition_variable cv_is_not_empty;

public:
    explicit MyQueue(int size) {
        this->sz = size;
        data = new T[size];
    }
    ~MyQueue() {
        delete[] data;
    }

    void push(T item) override {
        // all function body is critical section
        std::lock_guard<std::mutex> lock(m);

        // if queu is full replace the oldest elem (given by pop idx)
        if (elem_count == sz) {
            //push_idx = pop_idx;
            // update pop_idx to point to the new oldest item
            pop_idx = (pop_idx + 1) % sz;
        } else {
            elem_count+=1;
        }
        data[push_idx] = item;
        push_idx = (push_idx + 1) % sz;

        cv_is_not_empty.notify_one();
    }

    T pop() override {
        std::unique_lock lock(m);

        // wait for queue to be not empty
        while(empty())
        {
            cv_is_not_empty.wait(lock);
        }

        T elem = data[pop_idx];
        elem_count-=1;
        pop_idx = (pop_idx + 1) % sz;

        return elem;
    }

    T popWithTimeout(int milliseconds) override {
        std::unique_lock lock(m);

        // wait for queue to be not empty or for timeout
        while(empty())
        {
            auto res = cv_is_not_empty.wait_for(
                lock,
                std::chrono::milliseconds(milliseconds));

            if (res == std::cv_status::timeout)
            {
                throw std::runtime_error("Timeout in popWithTimeout");
            }
        }

        T elem = data[pop_idx];
        elem_count--;
        pop_idx = (pop_idx + 1) % sz;

        return elem;
    }

    int count() override {
        return elem_count;
    }

    int size() override {
        return this->sz;
    }

    bool empty() {
        return elem_count == 0;
    }

    void print_status() {
        printf("Count: %d | push_idx: %d | pop_idx: %d\n", this->elem_count, this->push_idx, this->pop_idx);
    }

};

#endif //MYQUEUE_H
