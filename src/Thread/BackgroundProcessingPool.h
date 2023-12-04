#pragma once

#include <thread>
#include <map>
#include <vector>
#include <ctime>
#include <shared_mutex>
#include <mutex>
#include <atomic>
#include <iostream>

class BackgroundProcessingPoolTaskInfo;

enum class BackgroundProcessingPoolTaskResult {
    SUCCESS,
    ERROR,
    NOTHING_TO_DO,
};

class BackgroundProcessingPool {
public:
    friend class BackgroundProcessingPoolTaskInfo;

    using Threads = std::vector<std::thread>;
    using TaskResult = BackgroundProcessingPoolTaskResult;
    using Task = std::function<TaskResult()>;
    using TaskInfo = BackgroundProcessingPoolTaskInfo;
    using TaskHandle = std::shared_ptr<TaskInfo>;

    BackgroundProcessingPool(size_t size_);

    ~BackgroundProcessingPool();

    size_t getNumberOfThreads() const {
        return size;
    }

    TaskHandle addTask(const Task &task);

    TaskHandle createTask(const Task &task);

    void startTask(const TaskHandle &task, bool allow_execute_in_parallel = true);

    void removeTask(const TaskHandle &task);

public:
    using Tasks = std::multimap<time_t, TaskHandle>;

    void workLoopFunc();

    void rescheduleTask(Tasks::iterator &task_it, const time_t &new_scheduled_ts) {
        auto node_handle = tasks.extract(task_it);
        node_handle.key() = new_scheduled_ts;
        task_it = tasks.insert(std::move(node_handle));
    }

    const size_t size;
    Threads threads;
    Tasks tasks;
    bool shutdown{false};

    std::mutex tasks_mutex;

};

class BackgroundProcessingPoolTaskInfo {
public:
    friend class BackgroundProcessingPool;

    BackgroundProcessingPoolTaskInfo(BackgroundProcessingPool &pool_,
                                     const BackgroundProcessingPool::Task &task_function_)
            : pool(pool_), task_function(task_function_) {}

private:
    BackgroundProcessingPool &pool;
    BackgroundProcessingPool::Task task_function;

    std::atomic<bool> remove{false};
    std::shared_mutex rwlock;

    BackgroundProcessingPool::Tasks::iterator iterator;

    bool allow_execute_in_parallel = false;
    size_t concurrent_executors = 0;
};
