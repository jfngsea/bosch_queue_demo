//
// Created by jfngsea on 2/5/25.
//

#ifndef QUEUE_H
#define QUEUE_H

template <typename T>
class Queue {
public:
    virtual ~Queue() = default;

    virtual void push(T item)=0;
    virtual T pop()=0;
    virtual T popWithTimeout(int milliseconds)=0;
    virtual int count()=0;
    virtual int size()=0;
};

#endif //QUEUE_H
