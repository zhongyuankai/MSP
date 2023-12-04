#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <memory>
#include <thread>

class ThreadPool {
public:
    using Task = std::function<void()>;


    ThreadPool(int max_thread_num_, int max_task_num_)
            : max_thread_num(max_thread_num_), max_task_num(max_task_num_) {

        threads.resize(max_thread_num);
        for (auto &thread : threads) {
            thread = std::thread([this] { workLoopFunc(); });
        }
    }

    ~ThreadPool() {
        task_cond.notify_all();
    }

    bool submit(Task task) {
        if (tasks.size() >= max_task_num) {
            return false;
        }

        std::unique_lock<std::mutex> lock(task_mutex);
        tasks.push(task);
        task_cond.notify_one();
        return true;
    }

private:
    void workLoopFunc() {
        while (!shut_down) {
            Task task = nullptr;
            {
                std::unique_lock<std::mutex> lock(task_mutex);
                while (tasks.empty()) {
                    task_cond.wait(lock);
                    if (shut_down) {
                        return;
                    }
                }

                auto task = tasks.front();
                tasks.pop();
            }
            task();
        }
    }

private:
    int max_thread_num;
    int max_task_num;

    volatile bool shut_down = false;

    std::vector<std::thread> threads;
    std::queue<Task> tasks;
    std::mutex task_mutex;
    std::condition_variable task_cond;
};


