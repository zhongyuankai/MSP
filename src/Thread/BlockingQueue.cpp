//
// Created by didi on 2021/7/13.
//

#include <Thread/BlockingQueue.h>
#include <iostream>

template <class T>
void BlockingQueue<T>::put(const T& x) {
    std::lock_guard<std::mutex> locker(mutex);

    while(full())
    {
        std::cout << "the blocking queue is full,waiting..." << std::endl;
        not_full.wait(mutex);
    }

    std::cout << "put " << x  << std::endl;
    queue.push_back(x);
    not_empty.notify_one();
}

template <class T>
T BlockingQueue<T>::take() {
    std::lock_guard<std::mutex> locker(mutex);

    while(empty())
    {
        std::cout << "the blocking queue is empty,waiting..." << std::endl;
        not_empty.wait(mutex);
    }

    T x = queue.front();
    queue.pop_front();
    not_full.notify_one();
    std::cout << "take  " << x  << std::endl;
    return std::move(x);
}