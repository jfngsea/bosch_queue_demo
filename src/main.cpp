#include "Queue.h"
#include "MyQueue.h"

int consumer_thread(MyQueue<int> * queue);
int producer_thread(MyQueue<int> * queue);

int main() {


    int queue_size = 2;

    MyQueue queue = MyQueue<int>(queue_size);


    printf("Launching Threads...\n");


    std::thread consumer(consumer_thread, &queue);
    std::thread producer(producer_thread, &queue);

    consumer.join();
    producer.join();

    printf("All Threads joined!\n");


    return 0;
}

int consumer_thread(MyQueue<int> * queue) {
    //return 0;
    printf("Consumer thread started\n");
    int n = 0;
    n = queue->pop();
    printf("Consumer popped: %d\n", n);


    printf("Consumer thread sleeping for 1 sec\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    printf("Consumer thread awake\n");

    // n =3
    n = queue->pop();
    printf("Consumer popped: %d\n", n);

    // n=4
    n = queue->pop();
    printf("Consumer popped: %d\n", n);

    // n=5 (waits for producer)
    n = queue->pop();
    printf("Consumer popped: %d\n", n);




    printf("Consumer thread ended\n");
    return 0;
}

int producer_thread(MyQueue<int> * queue) {
    printf("Producer thread started\n");

    queue->push(1);
    printf("Producer pushed: 1\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    queue->push(2);
    printf("Producer pushed: 2\n");

    queue->push(3);
    printf("Producer pushed: 3\n");

    queue->push(4);
    printf("Producer pushed: 4\n");


    printf("Producer sleeping for 1sec...\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    queue->push(5);
    printf("Producer pushed: 5\n");


    printf("Producer thread ended\n");
    return 0;
}


int my_consumer_thread(MyQueue<int> * queue) {
    printf("Consumer thread started\n");
    int n = queue->pop();
    printf("Consumer popped: %d\n", n);


    n = queue->popWithTimeout(100);
    printf("Consumer popped: %d\n", n);


    n = queue->popWithTimeout(1000);
    printf("Consumer popped: %d\n", n);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("Consumer thread ended\n");
    return 0;
}

int my_producer_thread(MyQueue<int> * queue) {
    printf("Producer thread started\n");
    std::this_thread::sleep_for(std::chrono::seconds(2));

    queue->push(2);
    queue->push(4);


    printf("Producer thread ended\n");
    return 0;
}
