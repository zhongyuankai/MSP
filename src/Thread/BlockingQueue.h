#pragma once

#include <list>
#include <mutex>
#include <condition_variable>

template<class T>
class BlockingQueue {
public:
    bool empty() {
        return queue.empty();
    }

    bool full() {
        return queue.size() == maxSize;
    }

public:
    BlockingQueue(int maxSize_) : maxSize(maxSize_) {}

    BlockingQueue() : maxSize(8) {}
    void put(const T& x);
    void test1(){};
    T take();

private:
    std::list<T> queue;
    std::mutex mutex;
    std::condition_variable_any not_empty;
    std::condition_variable_any not_full;
    int maxSize;
};


//int main() {
//    BlockingQueue<int> queue(2);
//    queue.put(10);
//    queue.take();
//    queue.put(20);
//    queue.put(11);
//    queue.put(12);
//    return 0;
//}
