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

        if (elem_count == sz) {
            data[pop_idx] = item;
            pop_idx = (pop_idx + 1) % sz;
        } else {
            data[elem_count++] = item;
        }
    }

    T pop() override {
        std::unique_lock lock(m);

        // wait for queue to be not empty
        while(empty())
        {
            cv_is_not_empty.wait(lock);
        }

        T elem = data[pop_idx];
        elem_count--;
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

    inline bool empty() {
        return elem_count == 0;
    }

};

#endif //MYQUEUE_H
