//
// Created by jfngsea on 2/8/25.
//

#ifndef MYQUEUE_H
#define MYQUEUE_H

#include <stdexcept>
#include <mutex>
#include <condition_variable>

#include "Queue.h"

/**
 * Implementation of a Queue implemented using ring buffers.
 * This Queue stores a given amount of elements with push evicting the oldest pushed element (LRU).
 *
 * @tparam T Type to store in the queue
 */
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
    /**
     * Initializes the Queue
     * @param size The maximum ammount of elements to be stored.
     */
    explicit MyQueue(int size) {
        this->sz = size;
        data = new T[size];
    }
    ~MyQueue() {
        delete[] data;
    }

    /**
     * Pushes a new item into the queue.
     * If the queue is already full, the least recent element is resplaced.
     * @param item Item to push into the queue
     */
    void push(T item) override {
        // all function body is critical section
        std::lock_guard<std::mutex> lock(m);

        // if queu is full update pop_idx to point to the new oldest item
        if (elem_count == sz) {
            pop_idx = (pop_idx + 1) % sz;
        } else {
            elem_count+=1;
        }
        // add element into the queue
        data[push_idx] = item;
        push_idx = (push_idx + 1) % sz;

        cv_is_not_empty.notify_one();
    }

    /**
     * Pops the least recent element from the queue, blocking if the queue is empty.
     * @return item from the queue.
     */
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

    /**
     * Pops the least recent element from the queue,
     * blocking if the queue is empty until the specified time in milliseconds.
     * @param milliseconds The  given timeout time
     * @return item from the queue.
     */
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

    /**
     *
     * @return the numbers of elements stored in the queue
     */
    int count() override {
        return elem_count;
    }

    /**
     *
     * @return the size of the queue.
     */
    int size() override {
        return this->sz;
    }

    /**
     *
     * @return True if the queue has no elements, false otherwise
     */
    bool empty() {
        return elem_count == 0;
    }
};

#endif //MYQUEUE_H
