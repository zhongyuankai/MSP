//
// Created by didi on 2021/7/22.
//

#include "BackgroundProcessingPool.h"

BackgroundProcessingPool::BackgroundProcessingPool(size_t size_)
        : size(size_) {

    threads.resize(size);

    for (auto &thread : threads)
        thread = std::thread([this] { workLoopFunc(); });
}

BackgroundProcessingPool::~BackgroundProcessingPool() {
    try {
        {
            std::lock_guard lock(tasks_mutex);
            shutdown = true;
        }

        for (auto &thread : threads)
            thread.join();
    }
    catch (...) {
    }
}

BackgroundProcessingPool::TaskHandle BackgroundProcessingPool::addTask(const Task &task) {
    TaskHandle res = createTask(task);
    startTask(res);
    return res;
}

BackgroundProcessingPool::TaskHandle BackgroundProcessingPool::createTask(const Task &task) {
    return std::make_shared<TaskInfo>(*this, task);
}

void BackgroundProcessingPool::startTask(const TaskHandle &task, bool allow_execute_in_parallel) {
    time_t current_time = time(0);

    task->allow_execute_in_parallel = allow_execute_in_parallel;

    {
        std::unique_lock lock(tasks_mutex);
        task->iterator = tasks.emplace(current_time, task);
    }
}

void BackgroundProcessingPool::removeTask(const TaskHandle &task) {
    // 原子的修改值，并且返回原来的值
    if (task->remove.exchange(true))
        return;

    {
        std::unique_lock wlock(task->rwlock);
    }

    {
        std::unique_lock lock(tasks_mutex);
        tasks.erase(task->iterator);
    }
}

void BackgroundProcessingPool::workLoopFunc() {
    time_t schedule_task_start_time;

    while (true)
    {
        std::cout << "==================workloopFunc================="  << std::this_thread::get_id()  << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));

        TaskResult task_result = TaskResult::ERROR;
        TaskHandle task;

        {
            std::unique_lock lock(tasks_mutex);

            while (!task && !shutdown) {
                for (const auto &[time, handle] : tasks) {
                    if (!handle->remove
                        && handle->allow_execute_in_parallel || handle->concurrent_executors == 0) {
                        task = handle;
                        schedule_task_start_time = time;
                        ++task->concurrent_executors;
                        break;
                    }
                }

                if (task) {
                    time_t current_time = time(0);

                    if (schedule_task_start_time > current_time)
                        continue;
                }
            }
        }

        if (shutdown)
            break;

        std::shared_lock rlock(task->rwlock);

        try {
            task_result = task->task_function();
        }
        catch (...) {}

        {
            std::unique_lock lock(tasks_mutex);

            if (shutdown)
                break;

            --task->concurrent_executors;

            if (task->remove)
                continue;

            time_t current_time = time(0);
            rescheduleTask(task->iterator, current_time + 5 * 1000);
        }
    }

}