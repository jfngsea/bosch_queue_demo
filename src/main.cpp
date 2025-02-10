#include "Queue.h"
#include "MyQueue.h"

int consumer_thread();
int producer_thread();

int main() {

    MyQueue queue = MyQueue<int>(2);
    return 0;
}
